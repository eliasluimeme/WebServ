<?php
// Check if a file is being uploaded
if (isset($_FILES['file'])) {
    $targetDirectory = './var/upload'; // Change to your desired directory
    $targetFileName = $targetDirectory . basename($_FILES['file']['name']);

    // Check if the file already exists
    if (file_exists($targetFileName)) {
        echo 'File already exists.';
    } else {
        // Move the uploaded file to the target directory
        if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFileName)) {
            echo 'File uploaded successfully!';
        } else {
            echo 'Error uploading file.';
        }
    }
} else {
    echo 'Please select a file to upload.';
}
?>
