#!/bin/bash

echo "🔄 Updating packages..."
sudo apt update && sudo apt upgrade -y

echo "🧽 Cleaning unused packages..."
sudo apt autoremove -y
sudo apt autoclean

echo "✅ Update and cleanup complete."
