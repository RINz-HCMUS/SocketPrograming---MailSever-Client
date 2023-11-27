# SocketProject - Mail Server & Client
## Email client program has UI. Email client connects to Email Server to send email (SMTP) and receive email in mailbox (POP3).

## A project in the Computer Network course of FITUS of the VNUHCM - University of Sciences

### Content request:
1. The system should facilitate email transmission using the SMTP protocol, enabling the specification of recipients in the To, CC, and BCC fields. Notably, when utilizing BCC, recipients listed in the To and CC fields should remain unaware of the BCC recipients, as their identities should not be revealed when reading the email. Additionally, the system should support the attachment of files to outgoing emails.

2. Email retrieval from the mail server (utilizing the POP3 protocol) must be executed accurately, ensuring that previously downloaded emails are not re-downloaded. Students are expected to devise solutions that maintain this requirement even when the Mail Client is closed and reopened.

3. Users should be able to categorize emails into specific folders based on configuration settings in the config file. This categorization can be performed according to the sender's address, subject, or keywords within the email content. By default, emails should be downloaded to the Inbox folder unless they match any configured filtering criteria.

4. The system should provide the ability to filter out spam keywords from both the email subject and content, relocating identified spam emails to the Spam folder during the email retrieval process from the server's Mailbox.

5. Email status management, distinguishing between read and unread emails, is essential. Students are expected to propose solutions that appropriately mark email statuses, ensuring correctness even when the Mail Client program is closed and reopened.

6. Advanced functionality includes automatically fetching emails from the mailbox every 10 seconds, with the ability to configure the time interval for this automatic email retrieval in the config file.
