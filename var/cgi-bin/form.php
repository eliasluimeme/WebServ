<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Retrieve data from the form fields
    $name = $_POST['name'];
    $email = $_POST['email'];

    // Display the received data
    echo "Received POST data:\n";
    echo "Name: $name\n";
    echo "Email: $email\n";
} else {
    echo "Invalid request method. Please use POST.";
}
?>
    