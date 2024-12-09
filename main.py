from PyQt5.QtWidgets import QApplication, QWidget, QDesktopWidget, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QMenu
from PyQt5.QtCore import Qt, QTimer
import sys

class DesktopEnvironment(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        # Создаем основное окно
        self.setGeometry(0, 0, 800, 600)
        self.setWindowTitle('Минимальная графическая среда')
        self.center()

        # Рабочий стол
        desktop_layout = QVBoxLayout()
        desktop_label = QLabel('Рабочий стол', self)
        desktop_label.setAlignment(Qt.AlignCenter)
        desktop_layout.addWidget(desktop_label)

        # Панель задач
        taskbar_layout = QHBoxLayout()
        menu_button = QPushButton('Меню', self)
        menu = QMenu(self)
        menu.addAction('Пункт 1')
        menu.addAction('Пункт 2')
        menu_button.setMenu(menu)

        # Время
        self.time_label = QLabel('00:00:00', self)
        timer = QTimer(self)
        timer.timeout.connect(self.update_time)
        timer.start(1000)

        taskbar_layout.addWidget(menu_button)
        taskbar_layout.addStretch(1)
        taskbar_layout.addWidget(self.time_label)

        main_layout = QVBoxLayout()
        main_layout.addLayout(desktop_layout)
        main_layout.addLayout(taskbar_layout)

        self.setLayout(main_layout)

    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())

    def update_time(self):
        from datetime import datetime
        current_time = datetime.now().strftime('%H:%M:%S')
        self.time_label.setText(current_time)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = DesktopEnvironment()
    ex.show()
    sys.exit(app.exec_())
