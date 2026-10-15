# DIY Dreame Remote

Samodzielny, otwartozrodlowy pilot dla robotow Dreame na **LCDWiki ES3C35P,
ESP32-S3 N16R8, 320 x 480**. Docelowo sterowanie lokalne po Matter, trzy roboty
i aktualizacje Wi-Fi. Projekt niezalezny od DIY Shelly Remote oraz sensorow.

**Biezaca wersja: `0.1.0-bringup`, firmware testowy plytki.**
Nie paruje jeszcze robotow i nie wysyla komend. Wi-Fi, Matter oraz instalator
OTA nie sa aktywne. Ekran wymienia planowane modele jako niesparowane; nie
symuluje mapy, baterii ani statusu robota. Nie jest to stabilne wydanie.

## Co jest w kodzie

- Port LCD ST77922 QSPI i zintegrowanego dotyku I2C, przeniesiony z projektu
  Shelly z zachowaniem licencji. Kontrola rzeczywistej pojemnosci Flash/PSRAM.
- Ciemny interfejs LVGL, test czterech naroznikow i kolorow, jasnosc 10-100%,
  zgaszenie podswietlenia i wybudzanie dotykiem (pierwszy dotyk jest konsumowany).
- Przenosny rejestr maksymalnie trzech robotow wedlug tozsamosci Matter,
  dynamiczne sciezki komend i kontrola wyniku przez odczyt zwrotny.
  To logika biblioteczna, jeszcze niepolaczona z transportem Matter ani UI robota.
- GitHub Actions: testy native ASan/UBSan i kompilacja konkretnej plytki,
  diagnostyka rozmiaru, obraz USB, sumy SHA-256, pliki licencji.

## Budowanie

Bring-up jest przypiety do **ESP-IDF v5.5.4**, LVGL **9.3.0** i sterownika
ST77922 **2.1.1**. To wersje portu sprzetowego, nie deklaracja zgodnosci z
aktualnym ESP-Matter main. Zobacz [architekture](docs/ARCHITECTURE.md).

```sh
# Po instalacji i aktywacji ESP-IDF v5.5.4:
idf.py -B build-board set-target esp32s3
idf.py -B build-board build
idf.py -B build-board size
idf.py -B build-board -p /dev/ttyACM0 flash monitor

# Testy logiki bez ESP-IDF, wymagany g++:
python3 tools/test.py --sanitize
```

[Instrukcja instalacji i testow](docs/INSTALL_AND_TEST_PL.md) opisuje obraz
polaczony pod **0x0** oraz ograniczenia. Instalacja USB zastepuje obecny
firmware pilota. Program nie zmienia oprogramowania robotow.

## Dokumentacja

- [Stan prac i dowody](PROJECT_STATUS.md)
- [Sprzet](docs/HARDWARE.md) i [profil plytki](boards/lcdwiki-es3c35p/README.md)
- [Architektura](docs/ARCHITECTURE.md)
- [Zgodnosc X20 / X40 / X50](docs/COMPATIBILITY.md)
- [Pierwsza proba Matter](docs/MATTER_TEST_PLAN.md)
- [OTA: projekt i obecne ograniczenia](docs/OTA.md)
- [Walidacja](docs/VALIDATION.md)

Kod projektu: MIT. Atrybucje i licencje zaleznosci: [NOTICE.md](NOTICE.md).
