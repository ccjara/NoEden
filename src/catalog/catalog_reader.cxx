#include "catalog/catalog.hxx"
#include "catalog/catalog_reader.hxx"
#include "catalog/material_reader.hxx"

struct CatalogFileType {
    static constexpr std::string_view Material = "material";
};

CatalogReader::CatalogReader(ThreadPool& thread_pool) : thread_pool_(thread_pool) {
}

std::unique_ptr<Catalog> CatalogReader::read(const std::string& base_path) {
    MaterialReader material_reader;
    std::mutex material_reader_mutex;

    std::atomic num_tasks = 0;
    std::mutex num_tasks_mutex;
    std::condition_variable task_cv;

    for (const auto& file : list_files(base_path)) {
        const auto result = thread_pool_.run([&, file] {
            auto exit_guard = std::experimental::scope_exit([&] {
                std::lock_guard lock(num_tasks_mutex);
                --num_tasks;
                task_cv.notify_one();
            });

            LOG_INFO("Parsing catalog file {}", file);
            auto parse_result = toml::parse_file(file);

            if (!parse_result) {
                LOG_ERROR("Failed to parse catalog file {}: {}", file, parse_result.error().description());
                return;
            }

            // determine file type
            auto table = parse_result.table();
            const auto file_table = table["file"].as_table();
            if (!file_table) {
                LOG_ERROR("Catalog file {} does not contain a 'file' table", file);
                return;
            }
            const auto type = file_table->get("type")->as_string();
            if (!type) {
                LOG_ERROR("File table of catalog file {} does not contain a 'type' field", file);
                return;
            }
            if (*type == CatalogFileType::Material) {
                std::lock_guard lock(material_reader_mutex);
                material_reader.add(file, std::move(table));
                LOG_INFO("Detected material catalog file {}", file);
            }
        });

        if (result == ErrStopping) {
            return nullptr;
        }

        ++num_tasks;
    }

    {
        std::unique_lock lock(num_tasks_mutex);
        task_cv.wait(lock, [&num_tasks] { return num_tasks == 0; });
    }

    auto catalog = std::make_unique<Catalog>();

    catalog->set_materials(material_reader.read_materials());

    return catalog;
}

std::vector<std::string> CatalogReader::list_files(const std::string& base_path) const {
    std::vector<std::string> files;
    for (const auto& entry : fs::recursive_directory_iterator(base_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".toml") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}
