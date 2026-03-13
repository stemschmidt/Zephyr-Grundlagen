Voraussetzungen:

1) git installiert (https://git-scm.com/install)
2) Docker Engine installiert (https://docs.docker.com/engine/install)
3) VSCode mit den Plugins "Dev Containers", "Serial Monitor", "C/C++" und "Cortex-Debug" installiert (https://code.visualstudio.com/download)
4) JLink Tools von Segger installiert (https://www.segger.com/downloads/jlink/)

Entwicklungsumgebung initialisieren:
1) Linux Bash oder WSL2 starten
2) Repository auschecken: git clone https://github.com/stemschmidt/Zephyr-Grundlagen.git
3) Ins Verzeichnis wechseln, VScode starten: cd Zephyr-Grundlagen; code .
4) Popup "Reopen in container" auswählen (oder links unten in VScode manuell starten) -> Image wird heruntergeladen
5) In der bash "west init -l manifest" aufrufen
6) In der bash "west update" aufrufen

Beispiele bauen:
1) Im Rootverzeichnis in der bash "source zephyr/zephyr-env.sh" aufrufen (notwending nach jedem Neustart des Containers)
2) In ein Beispielverzeichnis wechseln
3) In der bash "west build -p -b nrf54l15dk/nrf54l15/cpuapp" aufrufen (zum Bau nach einer Änderung reicht "west build", da die build-Umgebung schon mit -b konfiguriert wurde)

Alle Beispiele werden mit "west build -p -b nrf54l15dk/nrf54l15/cpuapp" gebaut (keine non-secure applications).