Voraussetzungen:

1) git installiert (https://git-scm.com/install)
2) Docker Engine installiert (https://docs.docker.com/engine/install)
3) VSCode mit den Plugins "Dev Containers", "Serial Monitor", "C/C++" und "Cortex-Debug" installiert (https://code.visualstudio.com/download)
4) JLink Tools von Segger installiert (https://www.segger.com/downloads/jlink/)

Entwicklungsumgebung initialisieren:
1) Linux Bash oder Ubuntu Distribution in WSL2 starten
2) Repository auschecken: git clone https://github.com/stemschmidt/Zephyr-Grundlagen.git -> wichtig bei WSL2: In der virtuellen Festplatte der Linux Distribution auschecken, nicht im Windows-Dateisystem!
3) Ins Verzeichnis wechseln, VScode starten: cd Zephyr-Grundlagen; code .
4) Popup "Reopen in container" auswählen (oder links unten in VScode manuell starten) -> Image wird heruntergeladen
5) In der bash "west init -l manifest" aufrufen
6) In der bash "west update" aufrufen
7) In der bash "west zephyr-export" aufrufen (nach jedem Container-Neustart erforderlich, da das Setting nicht gespeichert wird)

Beispiele bauen:
1) In ein Beispielverzeichnis wechseln
2) In der bash "west build -p -b nrf54l15dk/nrf54l15/cpuapp" aufrufen (zum Bau nach einer Änderung reicht "west build", da die Build-Umgebung bereits mit -b konfiguriert wurde. '-p' löscht das Buildverzeichnis bevor neu gebaut wird)

Alle Beispiele werden mit "west build -p -b nrf54l15dk/nrf54l15/cpuapp" gebaut (keine non-secure applications).