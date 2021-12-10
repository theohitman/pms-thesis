## Σε αυτό τον φάκελο θα αποθηκεύεται ο κώδικας (Sketches) για το ESP8266 

## Arduino IDE

Για να μπορέσουμε να κάνουμε upload κώδικα στο board (ESP8236) πρέπει να εγκαταστήσουμε το Arduino IDE. Γίνεται και με το Visual Studio Code προσθέτοντας το extension PlatformIO αλλά εδώ θα περιγράψω την διαδικασία με το Arduino IDE.

1. Κατεβάζουμε την εφαρμογή από το επίσημο [site](https://www.arduino.cc/en/software) για το λειτουργικό που έχουμε και την εγκαθιστούμε
2. Από τα Preferences του Arduino IDE κάνουμε paste το παρακάτω URL στο πεδίο **Additional Board Manager URLs** ώστε να μπορούμε να συνδεθούμε στο ESP8266
```bash
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
3. Από τα Tools > Board > Board Manager κάνουμε αναζήτηση το *esp8266* επιλέγοντας την τελευταία έκδοση και πατάμε **Install**
4. Αφού συνδέσουμε το board, επιλέγουμε πόρτα από τα Tools > Port > κάτι σε *usbserial* (είναι διαφορετικό σε κάθε υπολογιστή), αν δεν εμφανίζεται ίσως χρειαζόμαστε drivers (δείτε [εδώ](https://docs.wemos.cc/en/latest/ch340_driver.html))
5. Έπειτα από Tools > Board > Επιλέγουμε το board που έχουμε συνδέσει. Στην περίπτωσή μας είναι το: LOLIN(WEMOS) D1 R2 & mini 
6. Τέλος κάνουμε copy-paste τον κώδικα που θέλουμε να φορτώσουμε και πατάμε **Verify** για να κάνει το compile και **Upload** για να το φορτώσει στο board


## Sketches

Τα προγράμματα στο Arduino λέγονται και Sketches. Η διαδικασία για να φορτώσουμε στο ESP8266 ένα από τα παρακάτω Sketches είναι η εξής:

1. Κάνουμε copy-paste τον κώδικα από το git σε ένα νέο παράθυρο στο Arduino IDE
2. Κλικ στο κουμπί **Verify** για να γίνει compile ο κώδικας και 
3. Τέλος πατάμε το κουμπί **Upload** για να φορτώσει τον κώδικα στο ESP8266

### Blink_LED

Αναβοσβήνει το built in LED που βρίσκεται πάνω στο board. Το συγκεκριμμένο Sketch το χρησιμοποιούμε για να ελέγξουμε αν έχουμε επικοινωνία με το board και μπορούμε να του κάνουμε upload κώδικα.

### DHT11 Console

Δείχνει τις μετρήσεις θερμοκρασίας και υγρασίας του αισθητήρα DHT11 στην σειριακή κονσόλα του Arduino IDE. Το Sketch χρειάζεται τις βιβλιοθήκες DHT Sensor Library και Adafruit Unified Sensor της Adafruit. Για να τις προσθέσουμε πάμε από τα Tools > Manage Libraries... κάνουμε αναζήτηση και Install την τελευταία έκδοση. Η συνδεσμολογία των Pins για να δουλέψει ο κώδικας είναι:

* VCC από το DHT11 στο 3V3 του ESP8266
* GND από το DHT11 στο GND του ESP8266
* DOUT από το DHT11 στο D1 του ESP8266

Αφού κάνουμε **Verify** και **Upload** για να δούμε τις μετρήσεις ανοίγουμε το **Serial Monitor** από τα Tools. 