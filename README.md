# Σύστημα Ειδοποίησης, Παρακολούθησης και Διαχείρισης Θερμοκρασίας και Υγρασίας για ένα Data Center (Work In Progress)

Ένα ενσωματωμένο σύστημα συνδεδεμένο με αισθητήρες και πρόσβαση στο διαδίκτυο για ειδοποίηση αν οι ενδείξεις από τους αισθητήρες ξεπεράσουν κάποιο προκαθορισμένο όριο. Τα δεδομένα από τους αισθητήρες θα αποθηκεύονται και οπτικοποιούνται για μελλοντική αναφορά.

**Hardware Components:**

- Raspberry Pi 4
- ESP8266 microchip
- Temperature, Humidity and Rain-Drop Sensors
- Modem
- ~~WiFi Display Module~~

**Software Components:**

- Raspberry Pi OS
- Database (influxdb)
- Web Server (nginx)
- Firewall (OpenBSD)
- MQTT Server
- Application Server (Gunicorn)
- Certificate Authority
- Git
- Ansible
- Docker

--- 
## Α' Μέρος - Προετοιμασία του Raspberry 

Προαπαιτούμενα: 

1. Raspberry Pi, SD card, καλώδιο ethernet και τροφοδοσία
2. Download [Raspberry Pi OS](https://www.raspberrypi.com/software/)
3. SSH key pair ([How To Set up SSH Keys on a Linux / Unix System](https://www.cyberciti.biz/faq/how-to-set-up-ssh-keys-on-linux-unix/)) 
4. Ansible ([Installation Guide](https://docs.ansible.com/ansible/latest/installation_guide/index.html#installation-guide))

### 1. Εγκατάσταση λειτουργικού συστήματος

Ο πρώτος τρόπος είναι χρησιμοποιώντας το [Raspberry Pi Imager](https://www.raspberrypi.com/software/) που έχει γραφικό περιβάλλον και ο δεύτερος είναι από terminal με την εντολή **dd**. Βάζουμε την SD κάρτα στον υπολογιστή μας και τρέχουμε τις παρακάτω εντολές. Με την πρώτη εντολή βλέπουμε τον αριθμό του δίσκου που έχει πάρει η κάρτα. Στη συνέχεια την κάνουμε unmount και με την εντολή dd γράφουμε το image στην SD κάρτα. (ΠΡΟΣΟΧΗ να δώσουμε τον σωστό αριθμό δίσκου (rdisk?) στην εντολή dd. 

```bash
diskutil list
diskutil unmountDisk /dev/disk4
dd if=2021-10-30-raspios-bullseye-armhf-lite.img of=/dev/rdisk4 bs=1m
```

### 2. Ενεργοποίηση SSH

By default το ssh είναι απενεργοποιημένο στο raspberry. Το ενεργοποιούμε δημιουργώντας ένα κενό αρχείο με όνομα ssh στο root folder. Έπειτα κάνουμε unmount την SD κάρτα και την βάζουμε στο raspberry.  

```bash
touch /Volumes/boot/ssh
diskutil unmountDisk /dev/disk4
```

### 3. Εγκατάσταση του SSH Κey 

Αφού γίνει boot το raspberry τρέχουμε την παρακάτω εντολή. Έτσι θα μπορούμε να κάνουμε login στο raspberry χωρίς να εισάγουμε κωδικό πρόσβασης αλλά με το ιδιωτικό μας κλειδί (SSH key pair). 

```bash
ssh-copy-id pi@raspberrypi
```

### 4. Εγκατάσταση Docker και Docker-Compose

Με το παρακάτω playbook γίνονται οι εξής ενέργειες στο raspberry

* Απενεργοποίηση του password authentication. SSH μόνο με κλειδί (SSH key pair)
* Αναβάθμιση όλων των πακέτων του raspberry
* Εγκατάσταση απαραίτητων πακέτων για το docker
* Εγκατάσταση docker 
* Προσθήκη δικαιωμάτων στον χρήστη pi ώστε να τρέχει εντολές για το docker
* Εγκατάσταση docker-compose

```bash
ansible-playbook playbooks/docker-install.yml
```