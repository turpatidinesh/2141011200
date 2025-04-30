#!/bin/bash

# Assignment 1: Text Processing and Automation
# Author: SHAAN ROUT
# Regd No: 2141018101

# Log file to process
LOG_FILE="/var/log/auth.log"

# Output files
OUTPUT_FILE="login_attempts.txt"
ERROR_FILE="error_messages.txt"
echo "Processing login attempts and errors..."
# Ensure log file exists
if [ ! -f "$LOG_FILE" ]; then
    echo "Log file $LOG_FILE not found!"
    exit 1
fi
#Extract all login attempts (both successful and failed)
echo "Login Attempts:" > "$OUTPUT_FILE"
grep -Ei 'Failed password|Accepted password' "$LOG_FILE" >> "$OUTPUT_FILE"
#Extract attempted usernames
echo -e "\nAttempted Usernames:" >> "$OUTPUT_FILE"
grep "Failed password" "$LOG_FILE" | awk '{for(i=1;i<=NF;i++) if ($i=="for") print $(i+1)}' | sort | uniq >> "$OUTPUT_FILE"
#Extract error messages and clean them up using sed
echo "Extracting error messages..."
grep -i "error" "$LOG_FILE" | sed 's/^[[:space:]]*//' > "$ERROR_FILE"
echo "Done. Output written to:"
echo " - $OUTPUT_FILE (login attempts and usernames)"
echo " - $ERROR_FILE (error messages)"

