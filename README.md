# Σύστημα Ειδοποίησης, Παρακολούθησης και Διαχείρισης Θερμοκρασίας και Υγρασίας για ένα Data Center (Work In Progress)

Ένα ενσωματωμένο σύστημα συνδεδεμένο με αισθητήρες και πρόσβαση στο διαδίκτυο για ειδοποίηση αν οι ενδείξεις από τους αισθητήρες ξεπεράσουν κάποιο προκαθορισμένο όριο. Τα δεδομένα από τους αισθητήρες θα αποθηκεύονται και οπτικοποιούνται για μελλοντική αναφορά.

**Hardware Components:**

- Raspberry Pi 4
- ESP8266 microchips
- DHT11 Temperature & Humidity Sensors
- Rain-Drop Sensors
- Motion & Contact Sensors
- LEDs & Buzzer

**Software Components:**

- Raspberry Pi OS
- Database (influxdb)
- Visualization (grafana)
- MQTT Server (mosquitto)
- Orchestrator (node-red)
- ~~Firewall (OpenBSD)~~
- ~~Certificate Authority~~
- Git
- Ansible
- Docker

--- 
## Α' Μέρος - Προετοιμασία του Raspberry 

Στο πρώτο μέρος υλοποίησης θα προετοιμάσουμε το raspberry ώστε να μπορέσει να υποδεχθεί το σύνολο των εφαρμογών που απαιτούνται για το project. Ξεκινάμε με την εγκατάσταση του βασικού λειτουργικού συστήματος **Raspberry Pi OS Lite**. Στη συνέχεια ενεργοποιούμε την απομακρυσμένη πρόσβαση με **SSH**. Αναβαθμίζουμε το σύνολο των πακέτων που υποστηρίζονται από το λειτουργικό και τέλος γίνεται η εγκατάσταση του **Docker** και **Docker-Compose**. 


Προαπαιτούμενα: 

- Raspberry Pi, SD card, καλώδιο ethernet και τροφοδοσία
- [Raspberry Pi OS](https://www.raspberrypi.com/software/)
- SSH key pair ([How To Set up SSH Keys on a Linux / Unix System](https://www.cyberciti.biz/faq/how-to-set-up-ssh-keys-on-linux-unix/)) 
- Ansible ([Installation Guide](https://docs.ansible.com/ansible/latest/installation_guide/index.html#installation-guide))

### 1. Εγκατάσταση λειτουργικού συστήματος

Ένας τρόπος να γίνει η εγκατάσταση του λειτουργικού συστήματος είναι χρησιμοποιώντας το [Raspberry Pi Imager](https://www.raspberrypi.com/software/) που έχει γραφικό περιβάλλον ενώ ένας δεύτερος τρόπος είναι από terminal με την εντολή **dd**. Βάζουμε την SD κάρτα στον υπολογιστή μας και τρέχουμε τις παρακάτω εντολές. Με την πρώτη εντολή βλέπουμε τον αριθμό του δίσκου που έχει πάρει η κάρτα SD. Στη συνέχεια την κάνουμε unmount και με την εντολή dd γράφουμε το image στην SD κάρτα. (ΠΡΟΣΟΧΗ να δώσουμε τον σωστό αριθμό δίσκου (rdisk? or sd*) στην εντολή dd. 

MacOS
```bash
diskutil list
diskutil unmountDisk /dev/disk4
dd if=2021-10-30-raspios-bullseye-armhf-lite.img of=/dev/rdisk4 bs=1m
```

Linux
```bash
lsblk
dd if=2021-10-30-raspios-bullseye-armhf-lite.img of=/dev/sdc bs=1m
```

### 2. Ενεργοποίηση SSH

By default το ssh είναι απενεργοποιημένο στο raspberry. Το ενεργοποιούμε δημιουργώντας ένα κενό αρχείο με όνομα ssh στο root folder. Έπειτα κάνουμε unmount την SD κάρτα και την βάζουμε στο raspberry.  

```bash
touch /Volumes/boot/ssh
```

### 3. Εγκατάσταση του SSH Κey 

Αφού γίνει boot το raspberry τρέχουμε την παρακάτω εντολή. Έτσι θα μπορούμε να κάνουμε login στο raspberry χωρίς να εισάγουμε κωδικό πρόσβασης αλλά με το ιδιωτικό μας κλειδί (SSH key pair). 

```bash
ssh-copy-id pi@raspberrypi
```

### 4. Ορισμός Τοποθεσίας

Αφού κάνουμε login στο raspberry τρέχουμε την παρακάτω εντολή ορίζοντας έτσι την τοποθεσία μας ώστε να έχει το raspberry την σωστή ώρα. 

```bash
sudo timedatectl set-timezone Europe/Athens
```

### 5. Εγκατάσταση Docker και Docker-Compose


Με το παρακάτω playbook γίνονται οι εξής ενέργειες στο raspberry:

* Απενεργοποίηση του password authentication. SSH μόνο με κλειδί (SSH key pair)
* Αναβάθμιση όλων των πακέτων του raspberry
* Εγκατάσταση απαραίτητων πακέτων για το docker
* Εγκατάσταση docker 
* Προσθήκη δικαιωμάτων στον χρήστη pi ώστε να τρέχει εντολές για το docker
* Εγκατάσταση docker-compose

```bash
ansible-playbook playbooks/docker-install.yml
```

--- 
## Β' Μέρος - Προετοιμασία του ESP8266 

Σε αυτό το μέρος θα πρέπει να γίνει η **διασύνδεση** των components (αισθητήρες, led, κτλ.) με το ESP8266 καθώς και ο **προγραμματισμός** του. Στον φάκελο [/sketches](https://github.com/theohitman/pms-thesis/tree/main/sketches) θα βρείτε οδηγίες για την εγκατάσταση της εφαρμογής Arduino IDE, τον κώδικα και τον τρόπο που τον φορτώνουμε στο ESP8266 καθώς και σχεδιαγράμματα με την συνδεσμολογία των επιμέρους components. 

Προαπαιτούμενα: 

- ESP8266 microchips
- Καλώδιο microusb to usb
- Arduino IDE
- Αισθητήρες Humidity Sensor DHT11, Rain Sensor Module, PIR Sensor Module HC-SR501, Magnetic Reed Switch (δεν είναι απαραίτητο να έχουμε όλους τους αισθητήρες)

<img src="images/DHT_Sensor.jpeg" width=200> <img src="images/Rain_Sensor.jpg" width=200> <img src="images/PIR_Sensor.jpg" width=200> <img src="images/Reed_Switch.jpeg" width=200>

- Leds διαφορετικών χρωμάτων και buzzer

<img src="images/LEDs.jpg" width=200> <img src="images/buzzer.jpg" width=135>

--- 
## Γ' Μέρος - Stack Deployment