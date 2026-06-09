////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       counter_manager.h
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#ifndef COUNTER_MANAGER_H
#define COUNTER_MANAGER_H

#include <QObject>
#include <QAtomicInteger>
#include <QSettings>

class CounterManager : public QObject                                   ///< Класс CounterManager - Потокобезопасный синглтон для единого счётчика запросов
{
    Q_OBJECT
public:    
    static CounterManager& instance();                                  ///< Доступа к синглтону, возвращает единственный экземпляр
    void increment();                                                   ///< Увеличиваем счётчик на 1
    void reset();                                                       ///< Сбрасываем счётчик в 0
    quint64 value() const;                                              ///< Устанавлием текущее значение счётчика

private:    
    CounterManager();                                                   ///< Приватный конструктор (синглтон)
    ~CounterManager() = default;                                        ///< Деструктор по умолчанию

    CounterManager( const CounterManager& ) = delete;                   ///< Запрет копирования
    CounterManager& operator=( const CounterManager& ) = delete;        ///< Запрет присваивания

    void save();                                                        ///< Сохраняем текущее значение счётчика в QSettings
    QSettings m_settings;                                               ///< Хранилище для сохранения значения счётчика
    QAtomicInteger<quint64> counter;                                    ///< Атомарный счётчик (без блокировок)
};

#endif // COUNTER_MANAGER_H
