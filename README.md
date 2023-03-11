# Kill Feed Weapon Detection
This program detects weapons in a series of images, using a collection of templates. It uses the jsoncpp library to output the detected events in JSON format.
![alt text](https://github.com/[simonmark1998]/[detection]/blob/[master]/result.png?raw=true)

## Dependencies
- OpenCV 4
- jsoncpp
## Usage
1. Save the images to be analyzed in the **input_files** directory
2. Save the template images in the **templates** directory
3. Adjust the thresholds for each template in the **thresholds.txt** file
4. Compile and run the program
5. The output will be written to the console in JSON format

## Code Structure
The code is structured in the following way:

1. **Main function** - iterates through the images in the input directory, detects weapons and outputs the result in JSON format
2. **readWeapons** - reads the template images and stores them as Weapon objects
3. **setThresholds** - reads the thresholds for each template from a file and sets them in the corresponding Weapon object
4. **Weapon** - class that holds the template image, name, and threshold values for a specific weapon
## Functions
#### vector<Weapon> readWeapons()
- Reads the template images from the templates directory and stores them as Weapon objects
- Returns a vector of Weapon objects
#### void setThresholds(vector<Weapon> &weapons)
- Reads the threshold values for each template from a file and sets them in the corresponding Weapon object
- Modifies the threshold value of each Weapon object in the input vector
#### int main(int argc, char** argv)
- The main function that iterates through the images in the input directory, detects weapons and outputs the result in JSON format
- Reads the images from the input_files directory
- For each image, applies a region of interest to crop the top right corner of the image (where the kill feed is displayed)
- Converts the cropped image to grayscale and compares it to each template image using template matching
- If a match is found, records the weapon and the event time in a JSON object
- Outputs the JSON object to the console if any events are detected
## Classes
#### Weapon
Holds the template image, name, and threshold values for a specific weapon
Provides getters and setters for each variable
## External Libraries
#### OpenCV
Used for image processing
#### jsoncpp
Used for outputting the detection results in JSON format
