#pragma once

#include <string_view>

namespace ConsoleOutput {
// Единый формат ошибок; вызывать из catch. Цвет включается только для терминала.
void Error(std::string_view message);

// Удалённая позиция витрины: красный текст без выброса ошибки.
void Removed();

// Обычные пояснения о состоянии парка и витрины.
void Message(std::string_view message);
}  // namespace ConsoleOutput
