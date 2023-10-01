<?php
if (isset($_FILES['file'])) {
    $fileName = $_FILES['file']['name'];
    $targetDirectory = '/nfs/homes/iakry/Downloads/webserver-main/webserv/var/upload'; // Change to your desired directory
    $targetFilePath = $targetDirectory . $fileName;

    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFilePath)) {
        echo 'File uploaded successfully!';
    } else {
        header('HTTP/1.1 500 Internal Server Error');
        echo 'Error uploading file.';
    }
} else {
    header('HTTP/1.1 400 Bad Request');
    echo 'Invalid request. Please upload a file.';
}
?>
