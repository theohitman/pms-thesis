## Σε αυτό τον φάκελο θα αποθηκεύεται ο κώδικας (Sketches) για το ESP8266 

## Arduino IDE

Για να μπορέσουμε να κάνουμε upload κώδικα στο board (ESP8236)  πρέπει να εγκαταστήσουμε το Arduino IDE. Γίνεται και με το Visual Studio Code προσθέτοντας το extention PlatformIO αλλά εδώ θα περιγράψω την διαδικασία με το Arduino IDE.

1. Κατεβάζουμε την εφαρμογή από το επίσημο [site](https://www.arduino.cc/en/software) για το λειτουργικό που έχουμε και την εγκαθιστούμε
2. Από τα Preferences του Arduino IDE κάνουμε paste το παρακάτω URL στο πεδίο **Additional Board Manager URLs**
```bash
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
3. Από τα Tools > Board > Board Manager κάνουμε αναζήτηση το esp8266 επιλέγοντας την τελευταία έκδοση και πατάμε **Install**
4. Αφού συνδέσουμε το board, επιλέγουμε πόρτα από τα Tools > Port > κάτι σε *usbserial* (είναι διαφορετικό σε κάθε υπολογιστή), αν δεν εμφανίζεται ίσως χρειαζόμαστε drivers (δείτε [εδώ](https://docs.wemos.cc/en/latest/ch340_driver.html))
5. Έπειτα από Tools > Board > Επιλέγουμε το board που έχουμε συνδέσει. Στην περίπτωσή μας είναι το: LOLIN(WEMOS) D1 R2 & mini 
6. Τέλος κάνουμε copy-paste τον κώδικα που θέλουμε να φορτώσουμε και πατάμε **Verify** για να κάνει το compile και **Upload** για να το φορτώσει στο board


## Sketches

Τα προγράμματα στο Arduino λέγονται και Sketches

### Blink_LED

Αναβοσβήνει το built in LED που βρίσκεται πάνω στο board. Το συγκεκριμμένο Sketch το χρησιμοποιούμε για να ελέγξουμε αν έχουμε επικοινωνία με το board και μπορούμε να του κάνουμε upload κώδικα.