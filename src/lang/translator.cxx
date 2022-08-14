Translator::translation_dict Translator::db_;

void Translator::load(std::string_view lang_file) {
    const auto path = fs::absolute(fmt::format("dictionaries/{}", lang_file));
    const auto path_str = path.string();

    if (!fs::is_regular_file(path)) {
        Log::error("Language file at {} does not exist", path_str);
        return;
    }
    std::ifstream file(path_str.c_str());
    if (!file.is_open()) {
        Log::error("Could not open language file {}", path_str);
        return;
    }
    db_.clear();
    std::string line;
    while (std::getline(file, line)) {
        const auto off = line.find(':');
        if (off == line.npos) {
            continue;
        }
        db_.insert_or_assign(line.substr(0, off), line.substr(off + 1));
    }
    Log::info("Language file '{}' loaded", lang_file);
}
