# Makefile

# Définition du compilateur
CC = gcc

# Nom de l'exécutable
TARGET = main

# Fichiers objets nécessaires
OBJS = main.c calcul.c led.c

# Règle par défaut quand on tape "make"
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Règle pour nettoyer le projet quand on tape "make clean"
clean:
	rm -f $(TARGET)

WIN_PATH = /mnt/c/Users/hugo-/Downloads/Project_SysInfo

save:
	@echo "Sauvegarde..."
	cp -r . "$(WIN_PATH)"
