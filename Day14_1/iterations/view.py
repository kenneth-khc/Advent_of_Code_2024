import os
import keyboard

def read_file(file_number):
    """Reads and returns the content of the given file."""
    filename = f"iter{file_number}.txt"
    if os.path.exists(filename):
        with open(filename, "r") as file:
            return file.read()
    else:
        return f"File {filename} not found."

def display_file(file_number):
    """Displays the content of the given file."""
    os.system("cls" if os.name == "nt" else "clear")
    print(f"=== Displaying File: {file_number}.txt ===\n")
    print(read_file(file_number))
    print("\n=== Press Left/Right Arrow to Navigate or 'q' to Quit ===")

def main():
    current_file = 1
    while True:
        display_file(current_file)

        # Wait for key press
        event = keyboard.read_event()
        if event.event_type == "down":  # Only handle keydown events
            if event.name == "right":
                current_file = min(current_file + 1, 10000)
            elif event.name == "left":
                current_file = max(current_file - 1, 1)
            elif event.name == "q":
                os.system("cls" if os.name == "nt" else "clear")
                print("Exiting...")
                break

if __name__ == "__main__":
    print("Loading... Press Arrow Keys to Navigate.")
    print("Ensure files are named '1.txt' through '10000.txt'.\n")
    print("Press 'q' to quit.")
    keyboard.wait("right")  # Wait for the first keypress to start
    main()
