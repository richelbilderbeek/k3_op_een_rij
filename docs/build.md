# Building K3 Op Een Rij

On a terminal, do:

```bash
sudo apt-get install libqt5svg5-dev
git clone https://github.com/richelbilderbeek/K3OpEenRij.git
cd K3OpEenRij
./download_other_githubs.sh
qmake K3OpEenRijDesktop.pro
make
./K3OpEenRijDesktop
```
