#pragma once

class Catalog;
class ThreadPool;

class CatalogReader {
public:
    explicit CatalogReader(ThreadPool& thread_pool);

    /**
     * @brief Recursiely reads all catalog files from the given base path and constructs a catalog instance.
     *
     * While this method utilizes the thread pool to construct the catalog concurrently, this method will naturally
     * block until the catalog is fully constructed. It is recommended to offload this task to a separate thread.
     */
    std::unique_ptr<Catalog> read(const std::string& base_path);
private:
    std::vector<std::string> list_files(const std::string& base_path) const;

    ThreadPool& thread_pool_;
};
