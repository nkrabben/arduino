#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12

#define NUM_LEDS 60

#define BRIGHTNESS 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  
  Serial.println("Initializing display");
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.println("Connecting to Adafruit IO");
  // io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  // sunrise->onMessage(handleSun);
  // condition->onMessage(handleWeather);

  // wait for a connection
  //while(io.status() < AIO_CONNECTED) {
     // Serial.print(".");
    // delay(500);
  //}

  //Serial.println(io.statusText());
}

void loop() {
  //io.run();
  handleWeather(String("Snow"));
}




void handleWeather(String data) {
  
  String forecast = data; // store the incoming weather data in a string
  
  //the following strings store the varous IFTTT weather report words I've discovered so far
  String rain = String("Rain");
  String lightrain = String("Light Rain");
  String rainshower = String ("Rain Shower");
  String snow = String("Snow");
  String cloudy = String("Cloudy");
  String mostlycloudy = String("Mostly Cloudy");
  String partlycloudy = String("Partly Cloudy");
  String clearsky = String("Clear");
  String fair = String("Fair");
  String sunny = String("Sunny");
  String rainandsnow = String("Rain and Snow");
  String snowshower = String("Snow Shower");
  
  boolean precipitation;
  boolean cloud;
  String condition;
  uint8_t bg[4];
  uint8_t graysky[] = {10,10,10,10};
  uint8_t bluesky[] = {30,50,70,0};
  uint8_t precip[4];
  uint8_t snowcolor[] = {0,0,0,1};
  uint8_t raincolor[] = {0,0,1,0};
  int num_drops;
  int drop_length;
  int sun_space = floor(strip.numPixels()/6);
  int num_clouds = floor(sun_space/4);
  uint8_t clouds[2*num_clouds];


  if (forecast.equalsIgnoreCase(rain) || forecast.equalsIgnoreCase(lightrain) || forecast.equalsIgnoreCase(rainshower)){
    Serial.println("precipitation in the forecast today");
    precipitation = true;
    cloud = true;
    memcpy(bg, graysky, 4);
    memcpy(precip, raincolor, 4);
    num_drops = 7;
    drop_length = 6;
  }
  
  if (forecast.equalsIgnoreCase(snow) || forecast.equalsIgnoreCase(rainandsnow) || forecast.equalsIgnoreCase(snowshower)){
    Serial.println("precipitation in the forecast today");
    precipitation = true;
    cloud = true;
    memcpy(bg, bluesky, 4);
    memcpy(precip, snowcolor, 4);
    num_drops = 12;
    drop_length = 2;
  }
  
  if (forecast.equalsIgnoreCase(clearsky) || forecast.equalsIgnoreCase(fair) || forecast.equalsIgnoreCase(sunny)){
    Serial.println("some kind of sun in the forecast today");
    precipitation = false;
    cloud = false;
    memcpy(bg, bluesky, 4);
  }
  
  if (forecast.equalsIgnoreCase(cloudy) || forecast.equalsIgnoreCase(mostlycloudy) || forecast.equalsIgnoreCase(partlycloudy)){
    Serial.println("cloudy sky in the forecast today");
    precipitation = false;
    cloud = true;
    memcpy(bg, bluesky, 4);
  }

  Serial.println(num_drops);
  Serial.println(sun_space);
  uint8_t drops[num_drops];
  
  if (precipitation){
    for(int j=0; j<num_drops+1; j++) {
      drops[j]  = random(strip.numPixels());
    }
  }
  Serial.println(sizeof(drops));
  
  if(cloud){
    for(int i=0; i<num_clouds; i++){
      clouds[2*i] = random(sun_space);
      clouds[2*i+1] = clouds[2*i] + 1;
    }
  }
  
  for(int k = 0; k <256; k++){
    for(int i=sun_space; i<strip.numPixels(); i++){
      strip.setPixelColor(i, strip.Color(bg[0], bg[1], bg[2], bg[3]) );
    }
    for(int i=0; i<sun_space; i++){
      strip.setPixelColor(i, strip.Color(255, 90, 0, 0) );
    }
    if(cloud){
      for(int i=0; i<num_clouds; i++){
        strip.setPixelColor(clouds[2*i], strip.Color(0, 0, 0, 50) );
        strip.setPixelColor(clouds[2*i+1], strip.Color(0, 0, 0, 50) );
      }
    }
    if(precipitation){
      for(int j=0; j<num_drops; j++) {
        if(drops[j] > strip.numPixels()){
          drops[j] = sun_space;
        }
        for(int i=drop_length; i>0; i--){
          if(drops[j]-i > sun_space){
            int power = pow(2, (9 - floor((i * 8) / drop_length))) -1;
            strip.setPixelColor(drops[j]-i, strip.Color(0, 0, precip[2] * power, precip[3] * power ));
          }
        }
        drops[j] = drops[j] + 1;
      }
    }
    
    strip.show();
    delay(100);
  }
  
}


