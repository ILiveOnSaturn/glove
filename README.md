# Keyboard Glove

The project is about making a glove that can function as a keyboard, thus making it more accessible for physical work while writing.

To run the project on your own rp2040, you will need to do the following:
1. gather data about your handwriting using the training program.
2. train a model using the data you gathered.
3. add the model to the onboard code and compile.

After doing the following, you will be able to use the model as your own.

**Note:** You will need to download the raspberry pi pico sdk and add it into the CMakeLists.txt when compiling. you can download the pico sdk [here](https://github.com/raspberrypi/pico-sdk).

## Gathering Data:
To start gathering data, you will first need to load the training binary into the rp2040. to do that, you will need to add ProjectName.uf2 into the bootsel.

Connect the rp2040 into the computer and run the training program. Select the lower letters (the project currently doesn't support special characters and upper characters) and start writing. **Make sure the data and count files are deleted so you will have a fresh data file.**

**Disclamer!!: if you have a different imu than the L3GD20 LSM303D then you will need to change the code to match your own imu.**

(Note: if at any point you want to make changes to the training binary you can use cmake to build it. if you want to get back into the rp2040 bootsel, press and hold the button and insert the usb into your computer).

## Training the Model:
To train the model, run the ipynb notebook in the folder `glove/training program/` so it'll have access to your data.

## Adding the model and running it:
The only thing you need to do now is just to add the model.cc file into the `glove/onboard code/glove` folder and compile the code.

Before compiling the code make sure that:
1. The imu handler functions matche your own imu.
2. If you made any changes to the model then make sure all of the ops are imported in.

To compile the code run:
```
cd onboard\ code/
mkdir build
cd build

cmake ..
make
```

Now just drag and drop the uf2 file into the bootsel and have fun!
