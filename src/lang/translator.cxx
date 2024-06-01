#include "translator.hxx"

Translator::Translator(Dictionary&& dict) : dict_(std::move(dict)) {
}

