#include <SFML/Audio.hpp>

#include <iostream>

int main() {
    sf::Music music; // Создаем объект музыки

    if (!music.openFromFile("music.wav")) { // Загружаем файл
        return 1; // Если не удалось загрузить
    }

    music.setVolume(10); // Устанавливаем громкость (0-100)
    music.play(); // Начинаем воспроизведение
    music.setLoop(true); // Зацикливаем

    // Ждем, пока пользователь не нажмет Enter
    std::cout << "Playing music... Press Enter to stop.\n";
    std::cin.get();

    return 0;
}