# givemeattention
A gift to the special someone allowing them to request attention from a website which triggers a desk light

# How to use
1) Uses `docker-compose` to deploy with traefik at the moment, feel free to deploy it with anything else. The server is just a standard go mod based project
2) Make sure to point references to the api on the website frontend to your own domain name
3) Connect the arduino to your server over USB and update the port in the `main.go` file
4) Uses pin 6 (PWM Pin) on the Arduino for the WS2812 strip control, edit it if necessary
5) Profit!!
