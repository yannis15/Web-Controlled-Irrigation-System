# README Technique - Web-Controlled Irrigation System

## Introduction
Ce document décrit en détail l'installation, la configuration et la mise en œuvre du **Web-Controlled Irrigation System**. Il permet à toute personne, même débutante, de reproduire ce projet pas à pas.

## 1. Matériel Nécessaire
- **ESP32** : Microcontrôleur principal
<img src="https://drive.google.com/uc?export=view&id=1KtV_B2gryuFg4fIfBt_ODdlQqG09SlfY" alt=" ESP32" width="600" height="400"/>

- **Capteur d'humidité du sol** : Mesure l'humidité pour ajuster l'arrosage
<img src="https://drive.google.com/uc?export=view&id=1s6JVd6ANgd6GUYM2rHttRJSACztcdR0-" alt=" Capteur d'humidité" width="600" height="400"/>

- **Module RTC (DS3231)** : Gestion des horaires d'arrosage
<img src="https://drive.google.com/uc?export=view&id=1tth7EHTN3t9kEiDXoSwOo-qQovfIu-54" alt=" RTC " width="600" height="400"/>

- **Relais 5V** : Activation de la pompe
<img src="https://drive.google.com/uc?export=view&id=15eIQlDX9bJrOMAgfhmgpILsnE8D6Xdgk" alt=" Relais" width="600" height="400"/>

- **Pompe à eau** : Irrigation automatique
<img src="https://drive.google.com/uc?export=view&id=14pTBIdc-yZzLSE7-EKlOefr_ITjL0WAB" alt=" Pompe " width="600" height="600"/>

## 2. Schéma de Câblage
<img src="https://drive.google.com/uc?export=view&id=1rll9s8t1DwlUHFHZYO0cD9mKfmlu0l2v" alt="  " width="600" height="400"/>

## 3. Schéma de réalisation de maquette
- **ESP32** : Typon
<img src="https://drive.google.com/uc?export=view&id=1AdSpBTBcm6p3p2Q324PqbfT7RQ60tMxd" alt="  " width="600" height="400"/>
- **ESP32** : Schéma d'implentation des composants 
<img src="https://drive.google.com/uc?export=view&id=13_vcX1l7ta0_JAkblkWwJMUCaAFtWkaC" alt="  " width="600" height="400"/>
- **ESP32** : Vue 3D coté composant
<img src="https://drive.google.com/uc?export=view&id=1brYwziOkFD_ckcc1v3c3gsCF0QAZ9-hp" alt="  " width="600" height="400"/>
- **ESP32** : Vue 3D coté piste 
<img src="https://drive.google.com/uc?export=view&id=1fch02oOvClunEqyfsjRTrT0ZTr8fSTA1" alt="  " width="600" height="400"/>

### Connexions Principales
| Composant | ESP32 | Autres connexions |
|-----------|-------|------------------|
| Capteur d'humidité | GPIO15 | Alimentation 5V et GND |
| Module RTC DS3231 | SDA (GPIO19), SCL (GPIO18), RST (GPIO21) | Alimentation 5V |
| Relais | GPIO35 | Alimentation 5V et pompe |
| Pompe à eau | Relais | Alimentation 12V |

## 4. Installation Logicielle
### 4.1. Configuration de l'ESP32
1. Installer **Arduino IDE** et ajouter le **support ESP32**.
2. Installer les bibliothèques suivantes :
   - `WiFi.h` : Gestion du réseau version=3.1.1
   - `ESPAsyncWebSvr.h` : Serveur web asynchrone version=1.2.9
   - `AsyncTCP.h` : Serveur web asynchrone version=1.1.4
   - `ArduinoJson.h` : Traitement des données JSON  version=7.3.0
   - `Preferences.h` : Pour le stockage des données dans la mémoire EEPROM version=3.1.1
   - `virtuabotixRTC.h` : Gestion de l'horloge RTC
3. Flasher l'ESP32 avec le code source fourni dans le [repo GitHub](https://github.com/yannis15/Web-Controlled-Irrigation-System/blob/main/Src.ino).

### 4.2. Déploiement du Serveur Web
1. Renseigner dans le code source le nom et le mot de passe du point d'acces qui sera générer par l'esp32
2. Connecter votre appareil (téléphone portable ou ordinateur) au point d'accès.
3. Accéder à l'interface web via l'adresse IP affichée dans le moniteur série.
4. Configurer les horaires et modes d’arrosage depuis l’interface.

## 5. Calibration et Tests
### 5.1. Test des Capteurs
- Vérifier les valeurs du capteur d'humidité via le moniteur série.
- Tester l'activation manuelle de la pompe via l'interface web.

### 5.2. Vérification de l’Arrosage Automatique
- Régler une plage horaire et observer l’activation automatique de la pompe.
- Ajuster les seuils d'humidité pour une meilleure gestion de l’eau.

## 6. Déploiement et Améliorations
- **Ajout d’une application mobile** pour une gestion plus fluide.
- **Intégration de la météo en ligne** pour adapter l’arrosage aux précipitations prévues.
- **Stockage des données sur un serveur** pour une analyse des tendances d’irrigation avec de l'IA.

## 6. Ressources
- [Dépôt GitHub](lien_vers_le_repo)

Avec ce guide, vous devriez être en mesure de reproduire et d’adapter le projet en fonction de vos besoins. 🚀


