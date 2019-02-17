#Установка qt
sudo apt-get install qtcreator
#Установка компилятора g++
apt install g++-7
#Установка libssh2-1
apt install libssh2-1-dev


Структура проекта:
CameraIp - модель ip камеры. Содержит методы необходимые для работы с ip адресом
controller - основной модуль и модель программы. Графический интерфейс вызывает методы контроллера. Можно выкинуть QT интерфейс и прикрутить любой другой не изменяя controller и вспомогательные модули.
mainwindow - QT интерфейс
pingcontroller - содержит методы пинга
pngwidget - расширение qt QLabel с возможностью выделения ROI
sshcontroller - работает с библиоткетой ssh. Методы: init (вызывать в начале), shutdown (корректно все закрыть), command (выполнить команду по ssh), file (получить файл по scp).

Для корректной работы с файлами настроек на raspberry (через ssh) отключить пароль для sudo^
Open terminal window and type:

sudo visudo
In the bottom of the file, type the follow:

$USER ALL=(ALL) NOPASSWD: ALL


сборка libssh2
mkdir bin
  cd bin
  cmake ..
cmake --build .
cmake --build . --target install
