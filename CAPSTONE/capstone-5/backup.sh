#!/bin/bash

SOURCE="$HOME/Documents"
DEST="$HOME/backup"
mkdir -p "$SOURCE" "$DEST"
ARCHIVE="$DEST/backup_$(date +%F_%T).tar.gz"

tar -czf "$ARCHIVE" "$SOURCE"

if [ $? -eq 0 ]; then
    echo "✅ Backup created at $ARCHIVE"
else
    echo "❌ Backup failed!"
fi
