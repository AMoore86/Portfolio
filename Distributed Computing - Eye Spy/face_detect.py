import cv2 as cv

#Declaring classifier as haar cascade face detection
haar = cv.CascadeClassifier('haar_face.xml')

#Instantiate a recognizer using OpenCV
face_recognizer = cv.face.LBPHFaceRecognizer_create()
face_recognizer.read('faces_trained.yml')

#Authorization text
authorization = "Authorized"

#Video Input Capture [0 corresponds to laptop webcam]
cpt = cv.VideoCapture(0)

while True:
        # Read each frame of the video
        isTrue, frame = cpt.read()

        #Convert the frames to grayscale
        grayFrame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

        #Coordinates of face in the video
        faces_rect = haar.detectMultiScale(grayFrame, scaleFactor=1.1, minNeighbors=5)

        #Draw square locations where faces are found within the video
        for (x,y,w,h) in faces_rect:
            faces_region = grayFrame[y:y+h,x:x+h]
            cv.rectangle(grayFrame, (x,y), (x+w,y+h), (0,255,0), thickness=2)

        #Predict confidence for any face detected
        if(len(faces_rect) != 0):
            label, confidence = face_recognizer.predict(faces_region)

            #Send alert if unauthorized individual is detected
            if(confidence > 100):
                #Set video text to unauthorized
                authorization = "Unauthorized"
                
                #Send an alert to the customer                
                #Archive record of person detected

            #Display that authorized individual is detected
            else:
                #Set video text to authorized
                authorization = "Authorized"
                
                #Archive record of person detected
                
            #Display Authorization level of each person in frame
            for (x,y,w,h) in faces_rect:
                cv.putText(grayFrame, authorization, (x,y-5), cv.FONT_HERSHEY_COMPLEX, 1.0, (0,255,0), thickness=2)

        #Display video with rectangles
        cv.imwrite('Screenshot.jpg', grayFrame)
        
        #Stop reading if 'D' key is pressed
        if cv.waitKey(20) & 0xFF==ord('d'):
            break

#Stop capturing and remove video display window(s)
cpt.release()
cv.destroyAllWindows()

cv.waitKey(0)
