#!/bin/bash

while true; do
    echo ""
    echo "===== 🛠 Linux Maintenance Menu ====="
    echo "1. Backup ~/Documents"
    echo "2. Update & Clean System"
    echo "3. Monitor Logs"
    echo "4. Exit"
    echo "====================================="
    read -p "Choose an option [1-4]: " option

    case $option in
        1) bash backup.sh ;;
        2) bash update_cleanup.sh ;;
        3) bash log_monitor.sh ;;
        4) echo "👋 Exiting." ; exit ;;
        *) echo "❗ Invalid choice, try again." ;;
    esac
done
