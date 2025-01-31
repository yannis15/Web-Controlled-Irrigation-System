# README Technique - Web-Controlled Irrigation System

## Introduction
Ce document décrit en détail l'installation, la configuration et la mise en œuvre du **Web-Controlled Irrigation System**. Il permet à toute personne, même débutante, de reproduire ce projet pas à pas.

## 1. Matériel Nécessaire
- **ESP32** : Microcontrôleur principal
- **Capteur d'humidité du sol** : Mesure l'humidité pour ajuster l'arrosage
- **Module RTC (DS3231)** : Gestion des horaires d'arrosage
- **Relais 5V** : Activation de la pompe
- **Pompe à eau** : Irrigation automatique
- **Alimentation 12V** : Alimente le système
- **Résistances et câblage** : Connexions entre les composants
- **Boîtier** : Protection et organisation du circuit

## 2. Schéma de Câblage
*(Insérer ici un schéma détaillé des connexions du système)*

### Connexions Principales
| Composant | ESP32 | Autres connexions |
|-----------|-------|------------------|
| Capteur d'humidité | GPIO34 | Alimentation 3.3V et GND |
| Module RTC DS3231 | SDA (GPIO21), SCL (GPIO22) | Alimentation 3.3V |
| Relais | GPIO25 | Alimentation 5V et pompe |
| Pompe à eau | Relais | Alimentation 12V |

## 3. Installation Logicielle
### 3.1. Configuration de l'ESP32
1. Installer **Arduino IDE** et ajouter le **support ESP32**.
2. Installer les bibliothèques suivantes :
   - `WiFi.h` : Gestion du réseau
   - `ESPAsyncWebServer.h` : Serveur web asynchrone
   - `ArduinoJson.h` : Traitement des données JSON
   - `RTClib.h` : Gestion de l'horloge RTC
3. Flasher l'ESP32 avec le code source fourni dans le [repo GitHub](lien_vers_le_code).

### 3.2. Déploiement du Serveur Web
1. Connecter l’ESP32 au Wi-Fi.
2. Accéder à l'interface web via l'adresse IP affichée dans le moniteur série.
3. Configurer les horaires et modes d’arrosage depuis l’interface.

## 4. Calibration et Tests
### 4.1. Test des Capteurs
- Vérifier les valeurs du capteur d'humidité via le moniteur série.
- Tester l'activation manuelle de la pompe via l'interface web.

### 4.2. Vérification de l’Arrosage Automatique
- Régler une plage horaire et observer l’activation automatique de la pompe.
- Ajuster les seuils d'humidité pour une meilleure gestion de l’eau.

## 5. Déploiement et Améliorations
- **Ajout d’une application mobile** pour une gestion plus fluide.
- **Intégration de la météo en ligne** pour adapter l’arrosage aux précipitations prévues.
- **Stockage des données sur un serveur** pour une analyse des tendances d’irrigation.

## 6. Ressources
- [Dépôt GitHub](lien_vers_le_repo)
- [Documentation complète](lien_vers_la_doc)

Avec ce guide, vous devriez être en mesure de reproduire et d’adapter le projet en fonction de vos besoins. 🚀


