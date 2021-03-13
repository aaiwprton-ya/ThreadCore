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
/*
 * Основной поток должен спать
 * Следовательно должен иметь свою con_var
 * Основной поток может быть разбужен  модулем ввода / вывода
 * Следовательно модуль вводв / вывода должен иметь доступ к con_var основного потока
 *
 * Исполнение команд при пробуждении
 * Если это команда выхода - завершение всех модулей
 * Мониторинг состояний модулей, при необходимости их перезапуск
 * Выборочная остановка модулей
 *
*/
    init_modules();
    // second initialization, start threads
    while (false)
    {

    }
    // join all threads
    return 0;
}

void System::include_module(Module *module)
{
    module->generate_packages(&packs);
    module->generate_interface(&ifaces);
    modules.insert(std::pair<std::string, Module*>(module->name, module));
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
