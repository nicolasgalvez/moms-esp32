#!/bin/bash
# Parse command line arguments or use defaults
if [ $# -eq 2 ]; then
    HTML_DIR=$1
    OUTPUT_DIR=$2
else
    HTML_DIR="html"
    OUTPUT_DIR="include/html"
fi

# Ensure the output directory existsts
mkdir -p $OUTPUT_DIR
# Find all HTML files in the HTML directory and its subdirectories
find $HTML_DIR -type f -name "*.html" | while read -r html_file; do
    # Get the relative path of the file
    relative_path=${html_file#$HTML_DIR/}
    # Get the base name of the file
    base_name=$(basename "$html_file" .html)
    # Create the subfolders in the destination directory as needed
    mkdir -p "$OUTPUT_DIR/$(dirname "$relative_path")"
    # Convert the HTML file to a header file
    xxd -i "$html_file" > "$OUTPUT_DIR/$relative_path.h"
done