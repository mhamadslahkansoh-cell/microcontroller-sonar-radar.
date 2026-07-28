import pygame
import serial
import math
import time

# =========================================
# SERIAL SETTINGS
# =========================================
PORT = "COM4"
BAUD = 115200

try:
    ser = serial.Serial(PORT, BAUD, timeout=0.05)
    print("Radar connected on", PORT)
except:
    print("Cannot connect to", PORT)
    exit()

# =========================================
# SCREEN SETTINGS
# =========================================
WIDTH = 800
HEIGHT = 800

pygame.init()

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("ESP8266 RADAR SYSTEM")

clock = pygame.time.Clock()

# Colors
BLACK = (0, 0, 0)
GREEN = (0, 255, 0)
DARK_GREEN = (0, 80, 0)
RED = (255, 0, 0)
WHITE = (255, 255, 255)

# Radar center
cx = WIDTH // 2
cy = HEIGHT - 80

radius = 300
MAX_DISTANCE = 50

# Store detected points
targets = []

angle = 90
distance = 0

font = pygame.font.SysFont(None, 35)

# =========================================
# FUNCTIONS
# =========================================
def polar_to_xy(angle, distance):
    r = (distance / MAX_DISTANCE) * radius
    rad = math.radians(angle)
    x = cx - r * math.cos(rad)
    y = cy - r * math.sin(rad)
    return int(x), int(y)

def draw_radar():
    for r in range(60, radius+1, 60):
        pygame.draw.circle(screen, DARK_GREEN, (cx, cy), r, 2)

    pygame.draw.line(screen, DARK_GREEN, (cx, cy), (cx-radius, cy), 2)
    pygame.draw.line(screen, DARK_GREEN, (cx, cy), (cx+radius, cy), 2)
    pygame.draw.line(screen, DARK_GREEN, (cx, cy), (cx, cy-radius), 2)

# =========================================
# MAIN LOOP
# =========================================
running = True

while running:
    screen.fill(BLACK)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    if ser.in_waiting:
        try:
            data = ser.readline().decode().strip()

            if "," in data:
                a, d = data.split(",")

                angle = int(float(a))
                distance = float(d)

                if distance < MAX_DISTANCE:
                    targets.append([angle, distance, time.time()])

        except:
            pass

    targets = [t for t in targets if time.time()-t[2] < 3]

    draw_radar()

    x, y = polar_to_xy(angle, MAX_DISTANCE)
    pygame.draw.line(screen, GREEN, (cx, cy), (x, y), 3)

    for t in targets:
        a, d, _ = t
        tx, ty = polar_to_xy(a, d)
        pygame.draw.circle(screen, RED, (tx, ty), 8)

    text = font.render(f"ANGLE: {angle} deg    DIST: {distance:.1f} cm", True, GREEN)
    screen.blit(text, (40, 40))

    title = font.render("ESP8266 RADAR ACTIVE", True, GREEN)
    screen.blit(title, (220, 90))
    
    if distance <= 20:
        alert_text = font.render("⚠️ TARGET BREACH ⚠️", True, RED)
        screen.blit(alert_text, (290, 40))

    pygame.display.update()
    clock.tick(60)

ser.close()
pygame.quit()
