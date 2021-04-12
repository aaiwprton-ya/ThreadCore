#include "system.h"

namespace Core {

System::System()
{
    // first initialization
    // make package pool (need unique key)
}

System::~System() {}

int System::exec()
{
    init_modules();
    return 0;
}

void System::include_module(Module *module)
{
    module->generate_packages(&packs);
    module->generate_interface(&ifaces);
    modules.insert(std::pair<MODULE_NAME, Module*>(module->name, module));
}

void System::init_modules()
{
    for (auto &item: modules) {
        item.second->link_interfaces(&ifaces, &modules); }
    for (auto &item: modules) {
        item.second->generate_thread(&threads, &ifaces, &packs, &modules); }
    for (auto &item: modules) {
        item.second->link_threads(&threads, &packs, &modules); }
    for (auto &item: modules) {
        ifaces[item.second->get_if_key()].start(); }
    const Thread::__BaseThreadKey__ *t_key = nullptr;
    for (auto &item: modules)
    {
        item.second->get_thread_key(t_key);
        threads[*t_key].join();
    }
    for (auto &item: modules) {
        item.second->disconnect(&threads, &packs, &modules); }
}

} // namespace Core
