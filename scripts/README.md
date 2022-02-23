# Βοηθητικά Scripts

### temp.sh

Εμφανίζει την θερμοκρασία της GPU κάθε 2 δευτερόλεπτα. 

### backup.sh
## Διαδικασία Backup

Για backup ολόκληρου του project αρκεί να έχουμε τον φάκελο **docker** του raspberry. Αυτός μπορεί να αποθηκεύεται σε ένα USB flash drive ή ένα NFS share. Στην διαδικασία που περιγράφεται παρακάτω το backup είναι ημερήσιο ενώ backup παλαιότερα της μιας εβδομάδας διαγράφονται αυτόματα. 
- [How to mount a USB drive on the Raspberry Pi?](https://raspberrytips.com/mount-usb-drive-raspberry-pi/)
- [Connecting to an NFS Share on the Raspberry Pi](https://pimylifeup.com/raspberry-pi-nfs-client/)

1. Δημιουργούμε ένα αρχείο backup στο παρακάτω path και αντιγράφουμε τα περιεχόμενα του script σε αυτό. Αλλάζουμε το περιεχόμενο της μεταβλητής **backup_path** με το path που θέλουμε να αποθηκεύεται το backup.

```bash
cd /etc/cron.daily
sudo vim backup
```
2. Μετατρέπουμε το αρχείο σε εκτελέσιμο

```bash
sudo chmod +x backup
```

## Διαδικασία Restore

1. Εκτελούμε όλα τα βήματα από το Α' Μέρος - Προετοιμασία του Raspberry

2. Εκτελούμε τα 3 πρώτα βήματα από το Γ' Μέρος - Stack Deployment 

3. Κατεβάζουμε το stack και διαγράφουμε τον φάκελο του docker

 ```bash
cd docker
docker-compose down
cd ..
sudo rm -rf docker
```

4. Αντιγράφουμε το .tgz αρχείο backup από το USB flash drive ή το NFS share που θέλουμε στον home folder του raspberry (/home/pi)

5. Τρέχουμε τις παρακάτω εντολές επαναφέροντας τον βασικό φάκελο του project και ξεκινάμε το stack 

 ```bash
tar xvf /mnt/usb1/project-backup-"date_of_backup".tgz 
cd docker
docker-compose up -d
```