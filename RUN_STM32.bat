@echo off
color 0b
echo ========================================================
echo   LANCEMENT DU DEPLOIEMENT STM32
echo ========================================================
echo.
echo Ce script va chercher automatiquement les outils ST,
echo compiler le projet, le flasher, et ouvrir le dashboard.
echo.
python Deploiement_Automatique.py
pause
