# BlackEagle – parental control service for all
<img align="right" src="https://i.ibb.co/rwB4fLv/be-logo.png" width="300" hspace="10" vspace="10" align="center">

## About
BlackEagle is a parental control system that helps parents monitor their children's computers and reduce their exposure to content that is not appropriate for their age, while putting great emphasis on increased security and privacy of the information stored on our system.

The system consists of three parts:
* Remote server developed using Spring-Boot in conjunction with Apache-Tomcat server software.
* C / C ++ spyware that is responsible for collecting data from the monitored computer and keeping it on a remote server securely.
* A web interface developed using React-Native that allows remote access to data collected from a remote computer, in order to detect inappropriate content.

## Server (Spring-Boot + Apache-Tomcat)
* The server side is developed in the Java language using Spring-Boot in combination with the Apache-Tomcat server software. 
* The development is done with the support of many external libraries, which are imported through Maven technology.
* The entities are saved in a PostgreSQL database using the CrudRepository interface that provides functions that themselves perform the desired queries on the database (saving, deleting, retrieving, etc.).
* As part of the system's security process, there is a system for managing permissions according to user types (for each User type entity that has the ability to connect to the system, there are unique permissions for it).
* The server is also responsible for sending updates to the personal email accounts of the system users.

## Spying Application
The spy software developed in C/C++ languages is responsible for collecting the data from the monitored computer and sending it to the remote server. In addition, the software is responsible for filtering and blocking inappropriate content and is able to perform the following actions:
* Capturing screenshots
* Keystroke logging
* Taking pictures through the mobile camera
* Audio recording
* Viewing the network traffic of the monitored computer
* Blocking inappropriate content
* Locating the location of the monitored computer
* Remote computer locking
* Sending CMD commands

The spyware connects to the remote server as a with reduced privileges, using login information that will be created by the parent when the device is created in the database.

## System Security
The existing protections in the system:
* **Access to information**
   * To prevent **Man in the Middle** attacks in which an attacker can listen to the network traffic and steal the user's information, all communication between the client side and the server side is encrypted using the **TLS 1.2 protocol**.
   * Information transmitted from the child's computer to the remote server is saved in an encrypted form using a **256-bit AES encryption key** and is only decrypted when the parent requests to view the data. <br /> The content of the encrypted file is encoded after the **Base64 encryption** and when it is retrieved from the server, it will be transmitted and encoded to the end user and decoded back by the user interface to allow the user to view it.
   * **Two-Factor Authentication** is enabled by default for all system users. When logging in, the user is asked to enter the email and enter the one-time password he received. For the user's convenience, the one-time password is short enough (7 digits) to enable quick and simple typing. <br />
In order to generate the random value, **Java Security library** is used, which allows generating a cryptographically secure value. <br /> 
The one-time passwords are saved for each user in a temporary database for one minute from the moment of creation. After verification, the temporary user password will be automatically deleted from the database.
   * When resetting the password, the user is asked to enter the email he registered with and copy the value he received into the user interface. The way the value is generated is similar to how the value is generated for two-step verification, but here the user receives an extremely long value (32 characters) that includes digits, special characters, and both upper and lower case letters.

* **Password Enforcement**
   * Preventing a **Dictionary Attack** by prohibiting the use of passwords that have been distributed in leaked databases in past attacks. The dictionary contains about 14 million leaked passwords.  <br /> 
For the efficiency of checking the match between the user's passwords and the dictionary database, we created a database of passwords that is initialized with HASH-SET when the server goes up.  <br /> 
This makes it possible to retrieve any password suggested by the user quickly from the dictionary if it exists there and accordingly ask the user to choose another password.
   * The passwords are stored in the database in encrypted form using the **SHA256 algorithm and an 8-byte salt value (SALT)**. For this purpose, **Spring-Security library and a function called Pbkdf2PasswordEncoder** are used which takes care of attaching the random salt value it generates to the hashed value created from the user's password. This allows the salt value to be retrieved from the password stored in the database.
   * The user must enter a **strong password** between 10 and 16 characters long that contains special characters, upper and lower case letters, and numbers.
   * In order to **prevent re-use of user passwords** from the past, a policy of saving password history up to 4 passwords back is applied on the server.
When changing a password by the user, the new password will be compared against the history and if a match is found, the user will be asked to choose a new password. If the password is not found in the history, the cached value of the previous password is added to the user's password list for future comparison.

* **Brute-force and Denial of Service**
   * After three failed login attempts the user's IP address will be blocked from login attempts for several minutes. After 10 failed login attempts, the user himself will be blocked in order to prevent targeted attacks on him.  <br />  In order to do this, we record every connection attempt in a temporary database where we save the IP address and the name of the user trying to connect. For each such attempt, we increase the number of attempts by one until reaching the connection attempt threshold that we have set at which you will be blocked. <br />
   * In order to prevent **Denial of Service Attacks**, each user is allocated a number of requests per minute of use which are frequently renewed every few seconds.  
 For this protection we use the open source library **Bucket4j** which allows us to define a "bucket" for each user with a maximum value of 30 requests, where for each user request the number of requests decreases by one until the bucket is completely empty. <br /> 
The filling of the bucket is done automatically every two seconds, which allows legitimate users to request a much higher number of requests than the allotted number, but at the same time prevents a potential attacker from requesting a quantity of requests that could bring down the service.

* **Role-based Authorization**
   * For each user there is a **ROLE** that defines the total number of operations he can perform on the server
   * A ROLE of the PLAYER type will not be able to perform actions that ADMIN performs, and a ROLE of the DEVICE type will not be able to perform actions that the parents perform.
   * A user who is only partially authenticated on the server using a primary password will have a ROLE of type TEMP_AUTH and without a secondary password from the email will not be able to perform any operation on the server except identification.
   * A user verified after a password reset request will receive a temporary RES_AUTH role that will be used to change his password.
   * The DEVICE type user does not have access to the parent's account and will keep all of the child's information in his personal account defined as an account fully owned by the parent.

* **SQL-Injections**
   * For protection against this type of attacks, we use the capabilities available in the **Spring-Security library**.
   * The system makes use of **Prepared Statements** for inputs transferred from the client side to the server side.
   * The prepared request takes the form of a template into which constant values are inserted during each input into the system. This method prevents the possibility of the input being part of the query request itself, thus preventing incorrect validation or performing other operations on the database.
 
* **Cross Site Scripting (XSS)**
   * For protection against this type of attacks, we use the capabilities available in the **Spring-Security library**. Spring-Security provides a number of HEADERS that can be used by default, among them the **X-XSS-PROTECTION Header** that instructs the browser to block any action that appears to be an XSS attack.
   * In some browsers such an X-XSS-PROTECTION header is not used and for these cases we use a feature called **Content Security Policy (CSP)**.  
