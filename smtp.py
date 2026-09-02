import smtplib
from email.message import EmailMessage

# Set up credentials (use an Apple App-Specific Password)
sender_email = "example.email@icloud.com" # Replace with actual email
app_password = "xxxx-xxxx-xxxx-xxxx"  # Replace with app-specific password

# Construct the email object
msg = EmailMessage()
msg["From"] = input("From: ")
msg["To"] = input("To: ")
msg["Subject"] = input("Subject: ")

print("Type your email body below (Press Ctrl+D on Mac/Linux or Ctrl+Z then Enter on Windows to send):")
body_lines = []
while True:
    try:
        line = input()
    except EOFError:
        break
    body_lines.append(line)

# Set the body text proper on the EmailMessage object
msg.set_content("\n".join(body_lines))

# Send via iCloud's SMTP server 
# If you have a different provider, will need to change the server
try:
    with smtplib.SMTP("smtp.mail.me.com", 587) as server:
        server.starttls()
        server.login(sender_email, app_password)
        server.send_message(msg)
        print("Email sent successfully!")
except Exception as e:
    print(f"Error sending email: {e}")