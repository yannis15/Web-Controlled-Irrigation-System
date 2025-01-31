/*




*/
#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>
#include <virtuabotixRTC.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>

// ***************************************************Créaction  de l'objet  serveur web**********************************************************************************************
AsyncWebServer server(80);

// ***************************************************Creation de l'objet RTC*********************************************************************************************************
virtuabotixRTC myRTC(18, 19, 21);

// ***************************************************Nom et mot de passe du point d’accès que nous créerons**************************************************************************
const char* ssid = "*******";
const char* password = "*******";

// ***************************************************Adresse IP fixe*****************************************************************************************************************
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// ***************************************************Variables pour stocker les heures de début et la durée d'arrosage***************************************************************************
String horaire1 = "";
String horaire2 = "";
String horaire3 = "";
int duree = 0;
bool arrosageEnCours = false;  // Variable globale pour l'état de l'arrosage
int hum = 2000;  // Seuil d'humidité
int cap_humidite = 15;
int pompe = 35;
int test = 2;  // La led de signalisation

// Instance pour la gestion des préférences NVS
Preferences preferences;

// Contenu HTML embarqué
    const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html lang="fr">
    <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Système d'Arrosage Automatique</title>
    <style>
    body {
    font-family: Arial, sans-serif;
    background-color: #f0f8ff;
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    margin: 0;
    }

    .container {
    background-color: white;
    padding: 20px;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    text-align: center;
    width: 80%;
    max-width: 600px;
    }

    h1 {
    color: #333;
    }

    form {
    display: flex;
    flex-direction: column;
    gap: 20px;
    }

    .periode {
    background-color: #e6f7ff;
    padding: 10px;
    border-radius: 8px;
    }

    label {
    display: block;
    margin-top: 10px;
    }

    input[type="time"],
    input[type="number"] {
    padding: 10px;
    border: 1px solid #ccc;
    border-radius: 4px;
    font-size: 16px;
    width: 100%;
    box-sizing: border-box;
    }

    button {
    padding: 10px 20px;
    border: none;
    border-radius: 4px;
    background-color: #28a745;
    color: white;
    font-size: 16px;
    cursor: pointer;
    }

    button:hover {
    background-color: #218838;
    }

    #message {
    margin-top: 20px;
    color: #28a745;
    }

    #etatArrosage {
    margin-top: 20px;
    color: red;
    font-weight: bold;
    }
    </style>
    </head>
    <body>
    <div class="container">
    <h1>Système d'Arrosage Automatique</h1>
    <form id="arrosageForm">
    <div class="periode">
    <label for="horaire1">Heure de début d'arrosage 1:</label>
    <input type="time" id="horaire1" name="horaire1" required>
    </div>

    <div class="periode">
    <label for="horaire2">Heure de début d'arrosage 2:</label>
    <input type="time" id="horaire2" name="horaire2" required>
    </div>

    <div class="periode">
    <label for="horaire3">Heure de début d'arrosage 3:</label>
    <input type="time" id="horaire3" name="horaire3" required>
    </div>

    <div class="periode">
    <label for="duree">Durée d'arrosage (minutes):</label>
    <input type="number" id="duree" name="duree" min="1" required>
    </div>

    <button type="submit">Sauvegarder</button>
    </form>
    <div id="message"></div>
    <div id="etatArrosage">État de l'arrosage: Inactif</div>
    </div>
    <script>
    document.getElementById('arrosageForm').addEventListener('submit', function(event) {
    event.preventDefault(); // Empêche la soumission du formulaire par défaut

    const startTimes = [
    document.getElementById('horaire1').value,
    document.getElementById('horaire2').value,
    document.getElementById('horaire3').value
    ];

    const duree = document.getElementById('duree').value;

    console.log('Start Times:', startTimes);
    console.log('duree:', duree);

    // Affiche un message de confirmation
    document.getElementById('message').innerText = "Arrosages programmés envoyés";

    // Envoi des données au serveur (ESP32)
    fetch('/set-timer', {
    method: 'POST',
    headers: {
    'Content-Type': 'application/json'
    },
    body: JSON.stringify({
    startTimes,
    duree
    })
    })
    .then(response => response.json())
    .then(data => {
    console.log('Succès:', data);
    // Maintient les valeurs après l'envoi
    document.getElementById('horaire1').value = startTimes[0];
    document.getElementById('horaire2').value = startTimes[1];
    document.getElementById('horaire3').value = startTimes[2];
    document.getElementById('duree').value = duree;
    })
    .catch((error) => {
    console.error('Erreur:', error);
    });
    });

    // Fonction pour actualiser l'état de l'arrosage
    function actualiserEtatArrosage() {
    fetch('/etat-arrosage')
    .then(response => response.json())
    .then(data => {
    const etatArrosage = data.arrosageEnCours ? "En cours" : "Inactif";
    document.getElementById('etatArrosage').innerText = "État de l'arrosage: " + etatArrosage;
    })
    .catch((error) => {
    console.error('Erreur:', error);
    });
    }

    // Actualiser l'état de l'arrosage toutes les 5 secondes
    setInterval(actualiserEtatArrosage, 5000);

    </script>
    </body>
    </html>
    )rawliteral";


 void splitTime(String time, int &hours, int &minutes) 
 // La page Web renvoie quatre variables: trois pour les horaires d'arrosage et une pour la durée. le contenu de chacune des variables est inutilisable telquel. 
 //Cette fonction permet tout d'abord de décomposer chaque variable afin de récupérer très exactement l'heure et la minute de chaque arrosage. La fonction les convertit ensuite en entier pour les comparaisons.
 {
    int separatorIndex = time.indexOf(':');
    hours = time.substring(0, separatorIndex).toInt();
    minutes = time.substring(separatorIndex + 1).toInt();
 }

void checkAndStartPump() 
{
    int currentHour = myRTC.hours;      // stocke l'heure courant fournie par le RTC
    int currentMinute = myRTC.minutes; // Stocke la minute fournie par le RTC

    int startHour1, startMinute1;
    int startHour2, startMinute2;
    int startHour3, startMinute3;

    // Faire un appel à la fonction splitTime
    splitTime(horaire1, startHour1, startMinute1);  
    splitTime(horaire2, startHour2, startMinute2);
    splitTime(horaire3, startHour3, startMinute3);

    // Lire la valeur du capteur d'humidité
    int soilMoisture =digitalRead(15);
    Serial.print("humidité :");
    Serial.println(soilMoisture);

    // Vérifiez et démarrez la pompe si l'heure actuelle correspond à une heure programmée et que le seuil d'humidité est atteint
    if (((currentHour == startHour1 && currentMinute == startMinute1) ||
         (currentHour == startHour2 && currentMinute == startMinute2) ||
         (currentHour == startHour3 && currentMinute == startMinute3)) &&
        soilMoisture == 1) {
        // Lancer la pompe
        digitalWrite(pompe, HIGH);
        digitalWrite(test, HIGH);
        arrosageEnCours = true; // Mettre à jour l'état de l'arrosage
        delay(duree * 60000); // Durée en millisecondes
        digitalWrite(pompe, LOW);
        digitalWrite(test, LOW);
        arrosageEnCours = false; // Mettre à jour l'état de l'arrosage
    }
}

void afficher_heure_date() {
    // Afficher la date et l'heure dans le moniteur série
    Serial.print("Current Date / Time: ");
    Serial.print(myRTC.dayofmonth);
    Serial.print("/");
    Serial.print(myRTC.month);
    Serial.print("/");
    Serial.print(myRTC.year);
    Serial.print("  ");
    Serial.print(myRTC.hours);
    Serial.print(":");
    Serial.print(myRTC.minutes);
    Serial.print(":");
    Serial.println(myRTC.seconds);
}

void loadSettings() {
    // Fonction pour charger les variable depuis l'EEPROM
    horaire1 = preferences.getString("horaire1", "00:00");
    horaire2 = preferences.getString("horaire2", "00:00");
    horaire3 = preferences.getString("horaire3", "00:00");
    duree = preferences.getInt("duree", 0);
/*
    Serial.println("Paramètres chargés:");
    Serial.println("Heure de début d'arrosage 1: " + horaire1);
    Serial.println("Heure de début d'arrosage 2: " + horaire2);
    Serial.println("Heure de début d'arrosage 3: " + horaire3);
    Serial.println("Durée d'arrosage: " + String(duree) + " minutes");
*/
}

void saveSettings() {
  // Fonction pour enrégister les variables dans l'EEPROM
    preferences.putString("horaire1", horaire1);
    preferences.putString("horaire2", horaire2);
    preferences.putString("horaire3", horaire3);
    preferences.putInt("duree", duree);
/*
    Serial.println("Paramètres sauvegardés:");
    Serial.println("Heure de début d'arrosage 1: " + horaire1);
    Serial.println("Heure de début d'arrosage 2: " + horaire2);
    Serial.println("Heure de début d'arrosage 3: " + horaire3);
    Serial.println("Durée d'arrosage: " + String(duree) + " minutes");
*/
}

void setup() {
    // Initialiser le port série pour le débogage
    Serial.begin(115200);

    // Initialiser les préférences NVS
    preferences.begin("arrosage", false);
    loadSettings(); // Charger les paramètres au démarrage

    // Configuration des broches
    pinMode(pompe, OUTPUT);
    pinMode(cap_humidite, INPUT);
    pinMode(test, OUTPUT);

    // Connectez-vous au Wi-Fi et configurez un point d'accès
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_IP, gateway, subnet);

    // Imprimer l'adresse IP
    Serial.print("Point d'accès IP:");
    Serial.println(WiFi.softAPIP());

    // Gérer la page de configuration
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });

    // Gérer la soumission du formulaire
    server.on("/set-timer", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<256> json;
        deserializeJson(json, data);

        JsonArray startTimes = json["startTimes"];
        duree = json["duree"];

        horaire1 = String((const char*)startTimes[0]);
        horaire2 = String((const char*)startTimes[1]);
        horaire3 = String((const char*)startTimes[2]);

        saveSettings(); // Sauvegarder les paramètres à chaque soumission

        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // Gérer la demande de l'état de l'arrosage
    server.on("/etat-arrosage", HTTP_GET, [](AsyncWebServerRequest *request) {
        String jsonResponse = "{\"arrosageEnCours\": " + String(arrosageEnCours ? "true" : "false") + "}";
        request->send(200, "application/json", jsonResponse);
    });

    // Démarrez le serveur
    server.begin();
}

void loop() {
    myRTC.updateTime();
    afficher_heure_date();
    checkAndStartPump();
    // Afficher les valeurs reçues
        Serial.println("Heure de début d'arrosage 1: " + horaire1);
        Serial.println("Heure de début d'arrosage 2: " + horaire2);
        Serial.println("Heure de début d'arrosage 3: " + horaire3);
        Serial.println("Durée d'arrosage: " + String(duree) + " minutes");
    delay(2000);

} 
