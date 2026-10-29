#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
#include <SPI.h>

// Define the pins used to control the Adafruit display
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   -1

#define ANALOG_PIN A0      // Analog output of the circuit

#define IR_LED_PIN  7      // IR LED control pin
#define RED_LED_PIN 6      // Red LED control pin

// Create an instance of the Adafruit_HX8357 display object
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// Define display parameters
#define SCREEN_WIDTH   480
#define SCREEN_HEIGHT  320

#define GRAPH_TOP      0
#define GRAPH_HEIGHT   160     // Upper half of the screen

#define TEXT_TOP       190

// The number of samples is equal to the screen width
#define N_SAMPLES SCREEN_WIDTH

// Time interval between two samples, in milliseconds
const int sampleDelay = 25;   

// Delay between the final plot and the start of a new acquisition
const int pauseAfterPlot = 5000; 

// Array used to store the signal acquired with the IR LED turned on
int irData[N_SAMPLES];        

// Array used to store the signal acquired with the red LED turned on
int redData[N_SAMPLES];       

// Array used to store the scaled y-coordinates for plotting the signal
int scaledData[N_SAMPLES];   

int x = 0;
int previousY = 0;

// Function prototypes
int findmax(int data[]);
int findmin(int data[]);
int findmean(int data[]);

void rescalearray(int data[]);
void plotarray(int data[]);

void acquireIRLive();
void acquireRED();

int calculateBPM(int data[]);
float calculateSpO2(int redData[], int irData[]);

void drawThresholdLine(int rawThreshold, int data[]);
void drawFinalText(int bpm, float spo2);

void setup() {
  // Configure the LED pins as outputs
  pinMode(IR_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  // here we configure the analog input pin
  pinMode(ANALOG_PIN, INPUT);

  // Turn off both LEDs at startup
  digitalWrite(IR_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);

  // here we initialize the display
  tft.begin();
  tft.setRotation(1);
  
  tft.fillScreen(HX8357_BLACK);

  // here we initialize the data arrays
  for (int i = 0; i < N_SAMPLES; i++) {
    irData[i] = 0;
    redData[i] = 0;
    scaledData[i] = GRAPH_TOP + GRAPH_HEIGHT / 2;
  }

  previousY = GRAPH_TOP + GRAPH_HEIGHT / 2;
}

void loop() {
  // Acquire the IR signal for the heart rate plot
  acquireIRLive();

  // Acquire the red LED signal to calculate the oxygen concentration
  acquireRED();

  // Calculate the final heart rate and oxygen concentration values
  int bpm = calculateBPM(irData);
  float spo2 = calculateSpO2(redData, irData);

  int threshold = findmean(irData);

  // Rescale and display the final IR graph
  rescalearray(irData);

  tft.fillScreen(HX8357_BLACK);

  plotarray(scaledData);

  drawThresholdLine(threshold, irData);

  drawFinalText(bpm, spo2);

  delay(pauseAfterPlot);

  // Clear the screen before starting a new measurement
  tft.fillScreen(HX8357_BLACK);

  x = 0;
  previousY = GRAPH_TOP + GRAPH_HEIGHT / 2;
}

// here we plot the heart rate graph during the acquisition 
void acquireIRLive() {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(IR_LED_PIN, HIGH);

  delay(100);

  tft.fillScreen(HX8357_BLACK);

  x = 0;
  previousY = GRAPH_TOP + GRAPH_HEIGHT / 2;

  while (x < N_SAMPLES) {
    int adcValue = analogRead(ANALOG_PIN);
    irData[x] = adcValue;

    int y = map(
      adcValue,
      0,
      1023,
      GRAPH_TOP + GRAPH_HEIGHT - 1,
      GRAPH_TOP
    );

    scaledData[x] = y;

    if (x > 0) {
      tft.drawLine(
        x - 1,
        previousY,
        x,
        y,
        HX8357_GREEN
      );
    }

    previousY = y;
    x++;

    delay(sampleDelay);
  }

  digitalWrite(IR_LED_PIN, LOW);
}

// Acquire the signal with the red LED turned on
// This acquisition is used for SpO2 calculation
void acquireRED() {
  digitalWrite(IR_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);

  delay(100);

  for (int i = 0; i < N_SAMPLES; i++) {
    redData[i] = analogRead(ANALOG_PIN);
    delay(sampleDelay);
  }

  digitalWrite(RED_LED_PIN, LOW);
}

// Find the maximum value in the array
int findmax(int data[]) {
  int maxValue = data[0];

  for (int i = 1; i < N_SAMPLES; i++) {
    if (data[i] > maxValue) {
      maxValue = data[i];
    }
  }

  return maxValue;
}

// Find the minimum value in the array
int findmin(int data[]) {
  int minValue = data[0];

  for (int i = 1; i < N_SAMPLES; i++) {
    if (data[i] < minValue) {
      minValue = data[i];
    }
  }

  return minValue;
}

// Calculate the mean value of the array
int findmean(int data[]) {
  long sum = 0;

  for (int i = 0; i < N_SAMPLES; i++) {
    sum += data[i];
  }

  return sum / N_SAMPLES;
}

// Rescale the acquired signal to fit the upper half of the screen
void rescalearray(int data[]) {
  int maxValue = findmax(data);
  int minValue = findmin(data);

  // If the signal is constant, draw a flat line in the middle of the graph area
  if (maxValue == minValue) {
    for (int i = 0; i < N_SAMPLES; i++) {
      scaledData[i] = GRAPH_TOP + GRAPH_HEIGHT / 2;
    }
    return;
  }

  // Map each acquired value between the minimum and maximum signal values
  for (int i = 0; i < N_SAMPLES; i++) {
    scaledData[i] = map(
      data[i],
      minValue,
      maxValue,
      GRAPH_TOP + GRAPH_HEIGHT - 1,
      GRAPH_TOP
    );
  }
}

// Plot the signal stored in the given array
void plotarray(int data[]) {
  for (int i = 1; i < N_SAMPLES; i++) {
    tft.drawLine(
      i - 1,
      data[i - 1],
      i,
      data[i],
      HX8357_GREEN
    );
  }
}

// Calculate the heart rate using the mean value of the IR signal as threshold
int calculateBPM(int data[]) {
  int minValue = findmin(data);
  int maxValue = findmax(data);
  int threshold = findmean(data);

  int amplitude = maxValue - minValue;

  // If the signal amplitude is too small, no reliable beat is detected
  if (amplitude < 10) {
    return 0;
  }

  int beatCount = 0;
  bool aboveThreshold = false;

  // Minimum distance between two beats to avoid counting the same peak twice
  int minDistanceSamples = 300 / sampleDelay;

  int lastBeatIndex = -minDistanceSamples;

  for (int i = 1; i < N_SAMPLES; i++) {
    if (data[i] > threshold && !aboveThreshold) {
      if (i - lastBeatIndex >= minDistanceSamples) {
        beatCount++;
        lastBeatIndex = i;
      }

      aboveThreshold = true;
    }

    if (data[i] < threshold) {
      aboveThreshold = false;
    }
  }

  float acquisitionTime = (N_SAMPLES * sampleDelay) / 1000.0;

  int bpm = (beatCount * 60.0) / acquisitionTime;

  return bpm;
}


float calculateSpO2(int redData[], int irData[]) {
  int redMax = findmax(redData);
  int redMin = findmin(redData);
  int redMean = findmean(redData);

  int irMax = findmax(irData);
  int irMin = findmin(irData);
  int irMean = findmean(irData);

  int acRed = redMax - redMin;
  int dcRed = redMean;

  int acIR = irMax - irMin;
  int dcIR = irMean;

  if (acRed <= 0 || acIR <= 0 || dcRed <= 0 || dcIR <= 0) {
    return 0;
  }

  float ratioRed = acRed / dcRed;
  float ratioIR = acIR / dcIR;

  if (ratioIR <= 0) {
    return 0;
  }

  float R = ratioRed / ratioIR;

  // Apply the SpO2 formula used in the assignment
  float spo2 = 97.94 + 1.15 * R;

  if (spo2 > 100) {
    spo2 = 100;
  }

  if (spo2 < 0) {
     spo2 = 0;
  }

  return spo2;
}

// Draw the red threshold line corresponding to the mean value
void drawThresholdLine(int rawThreshold, int data[]) {
  int minValue = findmin(data);
  int maxValue = findmax(data);

  if (maxValue == minValue) {
    return;
  }

  int yThreshold = map(
    rawThreshold,
    minValue,
    maxValue,
    GRAPH_TOP + GRAPH_HEIGHT - 1,
    GRAPH_TOP
  );

  tft.drawLine(
    0,
    yThreshold,
    SCREEN_WIDTH,
    yThreshold,
    HX8357_RED
  );
}

// Draw the final heart rate and oxygen saturation values
void drawFinalText(int bpm, float spo2) {
  tft.fillRect(
    0,
    GRAPH_HEIGHT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT - GRAPH_HEIGHT,
    HX8357_CYAN
  );

  tft.setTextColor(HX8357_RED);
  tft.setTextSize(2);

  tft.setCursor(60, TEXT_TOP);
  tft.print("Heart rate=");
  tft.print(bpm);
  tft.print(" bpm");

  tft.setCursor(60, TEXT_TOP + 40);
  tft.print("Saturazione O2=");
  tft.print(spo2, 1);
  tft.print(" %");
}