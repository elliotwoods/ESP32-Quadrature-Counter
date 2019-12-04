Quadrature counter for ESP32 / Expressif chips using the PCNT counter (no interrupts required).

I was having issues with lost counts on a rotary encoder (approx 750Hz rotations / 3000 Hz AB pulses).
The issue seemed to stem from interrupts being disabled for parts of the program run-time for other functon (e.g. printing to serial, software I2C screens).

With this counter, I get the same result as when I wasn't using serial, I2C, other, but I don't need to use any of my own interrupts so it never drops a beat.
