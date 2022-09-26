#include "ai_script.hxx"

void AiScript::clear() {
    // TODO: script invocation
}

AiNodeState AiScript::visit(AiContext& context) {
    return mod_state(AiNodeState::Succeeded);
}
