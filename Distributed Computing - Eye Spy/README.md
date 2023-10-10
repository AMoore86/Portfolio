# Distributed Computing - Eye Spy
## Goal
The goal of this program is utilize camera feed with facial recognition to alert users of any detection of unauthorized individuals. This project was a group effort in which I, Alex Moore, mainly focused on the facial recognition code, and connecting it to the other distributed sections of the project (backend, frontend, and alerts).

## The Program
### Overview
This project is distributed into multipl parts, including a hosted database (which is unfortunately no longer active because of payment necessities), a basic front-end webpage, a facial recognition program, and corresponding alert generation. The majority of the code included in this repository was completed by me (Alex Moore).

### Front-End
The webpage was implemented using HTML, CSS, and JavaScript. The page included some basic information about our software solution that we were implementing, some navigational links that did not lead to any other pages, and a login/signup modal. Because the main focus of this project was the actual facial recognition system, the functionality of the webpage was mostly non-existant aside from the login/signup modal. Using this modal, users could create submit information (such as name, email, phone number, and images of themself) in order to (mock) sign up for our software solution. This information would then be uploaded to our hosted database.

### Back-End
The hosted database was implemented using SQL with a paid subscription to AWS Cloud Computing Services. The database included a total of five tables:  
- **authorized_customer_mapping:** Includes mapping of customers who have subscribed to our software solution, and any person that their camera should include as an authorized individual.
- **cameras_installed:** Includes camera information including a unique identifier camera ID, location, address, and IP address.
- **customer_cam_mapping:** Includes "connection" between each customer ID and camera ID.
- **login_details:** Includes login information for front-end user accessibility including user ID, username, email, and password.
- **person_details:** Includes personal information including a unique identifier ID, name, email, phone number, address, and personal images.

### Facial Recognition
For this project, I implemented the facial recognition using an open-source computer vision library known as OpenCV. This library contains many features additional to the facial recognition that was utilized for this project. Using OpenCV and the included haar cascade xml file, I created a Python script (face_train.py) which trained a model using images of authorized individuals, correlating to each customer, by pulling the information from the SQL database. This was done by using an assortment of libraries, including OpenCV, NumPy, UrlLib, and MySQL.Connector. The resultant of the training includes NumPy files of the features and labels which will be utilized by the facial recognition.  
After training the model on these images, the facial recognition running in the main Python script (face_recognize.py) begins and continues to work as the camera records a live video feed. This script also pulls information regarding the camera and the authorized individuals associated with it. If any face is detected in the live feed, the program will then test whether the face correlates with an authorized individual or not. If the individual is unauthorized, the program will send an alert via a function in an alert generation script (Final_Text_Alert_Generation.py)  
The alert generation utilizes the personal contact information that the user inputted during signup in order to send an email and text message. This is done using various libraries including SMTP, SSL, and MIME.

### Current Functionality
The current functionality of the program is inoperative. This is mostly because of the now inactive SQL database; however, it is also a result of certain issues we had regarding the alert generation libraries. Additionally, the group member assigned to the front-end was never able to successfully host the website. I have also deleted the images of group members used for testing the program in order to maintain their anonymity (and replaced them with a folder of random images containing people).  

Because the overall program is no longer cohesive, I have attempted to create a mini (tutorial) version of the project using the face_detect.py script. Ideally, this script should open a window displaying a live feed of the user's laptop camera. Then if the program detects a face within the view of the camera, it should surround the face in a box and simply write "Authorized", otherwise it should display "Unauthorized".
