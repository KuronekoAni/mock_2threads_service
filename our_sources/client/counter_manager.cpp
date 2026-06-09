////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       counter_manager.cpp
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#include "counter_manager.h"   /// Заголовок класса

/// Конструктор: инициализируем счётчик значением из хранилища
CounterManager::CounterManager(): m_settings( "NAK_Company", "MockPollingClient" ), counter( m_settings.value( "counter", 0 ).toULongLong() )
{
}

/// Доступ к синглтону
CounterManager& CounterManager::instance()
{
    static CounterManager instance;
    return instance;
}

/// Увеличение счётчика на 1
void CounterManager::increment()
{
    ++counter;
    save();                             /// Сохраняем новое значение
}

/// Сброс счётчика в 0
void CounterManager::reset()
{
    counter.storeRelaxed( 0 );
    save();                             /// Сохраняем обнуление счётчика
}

/// Устанавлием текущее значение счётчика
quint64 CounterManager::value() const
{
    return counter.loadRelaxed();
}

/// Сохранение значения в QSettings
void CounterManager::save()
{
    /// Преобразуем quint64 в qulonglong для сохранения в настройках
    m_settings.setValue( "counter", static_cast<qulonglong>( counter.loadRelaxed() ) );
    /// Запись на диск
    m_settings.sync();
}
