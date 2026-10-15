# Instalacja i test 0.1.0-bringup

To test plytki, nie pilot sterujacy robotem. Nie ma Wi-Fi/Matter/OTA.
Dotyczy tylko LCDWiki ES3C35P ESP32-S3 N16R8.

1. Pobierz artefakt `lcdwiki-es3c35p-n16r8-bringup-NOT-STABLE` z udanej
   kompilacji GitHub Actions i rozpakuj. Sprawdz `SHA256SUMS.txt` oraz
   `BUILD_REPORT.json` (wlasciwy commit, hardware ID i wersja).
2. Pierwsza instalacja przez USB zastepuje obecny firmware pilota i obszary
   Flash objete obrazem, w tym ustawienia. Zachowaj poprzedni obraz do powrotu.
3. Wgraj **polaczony** `DIY-Dreame-Remote-es3c35p-bringup.bin` pod **0x0**:

```sh
python -m esptool --chip esp32s3 --port /dev/ttyACM0 --baud 460800 write_flash 0x0 DIY-Dreame-Remote-es3c35p-bringup.bin
```

Na Windows podaj wlasciwy port COM. Przewod USB-C musi obslugiwac dane.
Jesli trzeba: przytrzymaj BOOT, nacisnij/pusc RESET, pusc BOOT i ponow zapis.
Po instalacji nacisnij RESET. DIO / 80 MHz / 16 MiB Flash.
Nie zapisuj samej aplikacji `diy_dreame_remote.bin` pod 0x0.

Sprawdz na plytce:

- Pionowy ekran DIY DREAME REMOTE, trzy planowane modele, **none paired**.
- Menu -> Hardware tests: Flash 16 MiB / PSRAM 8 MiB, Touch responding.
- Check touchscreen: przyciski 1-4 odpowiadaja rzeczywistym naroznikom.
- Check display colors: podpisy RED/GREEN/BLUE/WHITE zgadzaja sie z kolorami.
- Settings: jasnosc 10-100%, Screen off, dotyk wybudza. Pierwszy dotyk tylko
  wybudza; kolejny uruchamia przycisk. Automatyczne wygaszanie po 60 s.
- Firmware Update uczciwie informuje, ze OTA nie jest jeszcze dostepne.
- 10 minut pracy i kilka cykli wygaszenia bez restartow. Bez dzialajacego
  dotyku wygaszanie jest wylaczone, aby ekran diagnostyczny pozostal dostepny.

Przekaz zdjecie ekranu, wynik naroznikow/kolorow/jasnosci/wybudzania,
oznaczenie rewizji PCB oraz log startowy USB. Nie publikuj hasel ani QR/PIN Matter.
Log ma wskazac `Flash=16777216 PSRAM=8388608` i `UI started`.
Samo przejscie kompilacji nie potwierdza testu fizycznego.
