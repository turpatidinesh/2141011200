#!/bin/bash

LOG="/var/log/dpkg.log"
KEYWORDS=("error" "fail" "critical")

echo "🔍 Monitoring $LOG for issues..."
> /tmp/log_alerts.txt

for word in "${KEYWORDS[@]}"; do
    grep -i "$word" "$LOG" >> /tmp/log_alerts.txt
done

if [ -s /tmp/log_alerts.txt ]; then
    echo "⚠️  Issues found in logs:"
    cat /tmp/log_alerts.txt
else
    echo "✅ No critical issues found."
fi
