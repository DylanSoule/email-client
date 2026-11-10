### Features
- Similar to SMTP implementation
- Additional features to retrieve an email from IMAP server
	- Not formatted and must manually change what email ID you want
### Research
---
In order to write the IMAP function I used documentation [here](https://curl.se/libcurl/c/imap-fetch.html), and my experience with libcurl from creating my basic smtp
### Code write up
---
I set up the IMAP implementation in the same libcurl wrapper class as the SMTP file, renaming it to EmailClient to make it more general. 
In order to set up the function I was able to use many of the same setup functions for libcurl, making the process much more smooth, the hardest part was ensuring the IMAP url was correct for Apple mail
```c++
std::string full = "imaps://" + std::string(imapServer_) + "/INBOX/;MAILINDEX=63";

curl_easy_setopt(curl, CURLOPT_URL, full.c_str());
```
I did this my passing the basic server(for apple it is imap.mail.me.com:993), ensuring to use port 993 for ssl imap through apple, otherwise connection will be refused for security reasons.
Additionally the mailindex is the emails in your inbox, where one is the oldest in your inbox, so it makes it difficult to know which one is first.
### Fetching the email
---
All of this allowed me to fetch an email from my inbox, to start I pulled a very simple text based one that just has a a subject line and one line of email.
![Email Preview](Assets/Email-Fetch-Preview.png)
When fetched to the terminal however it looks like this without any formatting:
```txt
[dylans@dylans-laptop:~/Projects/email-client]$ /home/dylans/Projects/email-client/smtp
Return-path: <dylan.soule@icloud.com>
Original-recipient: rfc822;dylan.soule@icloud.com
Received: from p00-icloudmta-asmtp-us-central-1k-100-percent-10 by p139-mailgateway-smtp-68dc5445cf-4744b (mailgateway 2634B97)
        with SMTP id d46c5731-b2bd-4246-beb5-8794eaaa4d6d 
        for <dylan.soule@icloud.com>; Wed, 23 Sep 2026 18:23:04 GMT
X-Apple-MoveToFolder: INBOX 
X-Apple-Action: MOVE_TO_FOLDER/INBOX
X-Apple-UUID: d46c5731-b2bd-4246-beb5-8794eaaa4d6d
Received: from outbound.ci.icloud.com (unknown [127.0.0.2])
        by p00-icloudmta-asmtp-us-central-1k-100-percent-10 (Postfix) with ESMTPS id 9A9F318017A2
        for <dylan.soule@icloud.com>; Wed, 23 Sep 2026 18:23:02 +0000 (UTC)
X-ICL-RepId: 01a0cf81-c15b-7524-9453-e8456fbce55c
X-ICL-Out-Info: HUtFAUMEWwJACksBTUQeDx5WFlZNRAJCTQhBCkMOXRxBAEgdXAJLVxQEHEoBVgUcA1YNXwh3AlEcVg1XQ1QEX1BfHA4EVAddBV1WUAJaS0ATBEkTBVIHXU1WDUcPWB5cFBcbWkNeCF4fTBwdDlgGEgBNCg42BlkFXglWA0MFNhIUXUVaDlsERxQXG1wAFwZbFAREVVkDXQUTDBweDARbBV0NHVFbGlNURA9VUQtSWFFCCkwDXwdZch1cVlACWlUSBEAIVlBeCF4fTBw=
Dkim-Signature: v=1; a=rsa-sha256; c=relaxed/relaxed; d=icloud.com; s=1a1hai; t=1790187784; x=1792779784; bh=cUG68bkLKRwbkXgCITcP5sGkcn9DMInvaChEjQLCbOU=; h=To:From:Subject:Date:Message-id:Content-Type:MIME-Version:x-icloud-hme; b=DUP1+8wyf8kufCsdKL3cpBRQ6Oc47TwhwPZRfy8SPLMcPWt3ythfywHT3DQXcDmTUc2WqRKFOp47ASoMawLzg0z0zDhZT2PFc5oDrOyzb44u6vh2B3/Sgsh0TpiarT0AlZQFQ8pZuHL7h9KoiRQhGEyUNaQtLVvw/TqpAtjON1pHVjTCnMq6jOckbDlnFFDeSwNNMtvploDpnJ05tf6JaonaqBUGwXjtz68W82xw/+O/piSJL/tSfJe55p620/WIRIG2NRSn3353o3oMzafLGk6Rr6YIpSGIYXjOKcD/hFfKmrgiWVU4arM4NSNfdMqL+6jd+E6R+2+CYM+IfOQlzQ==
Received: from p00-mailws2-65f7b486c7-v6sn7 (unknown [10.168.160.104])
        by p00-icloudmta-asmtp-us-central-1k-100-percent-10 (Postfix) with ESMTPSA id CA8AE1800A11
        for <dylan.soule@icloud.com>; Wed, 23 Sep 2026 18:23:01 +0000 (UTC)
To: dylan.soule@icloud.com
From: Dylan Soule <dylan.soule@icloud.com>
Subject: Fetch Test
Date: Wed, 23 Sep 2026 18:23:01 +0000 (GMT)
X-Mailer: iCloud MailClient2634Hotfix39 MailServer2612B5.952a5ad53a12
Message-id: <f4467c5d-a307-4eb6-8f46-bfe3c2340202@me.com>
Content-Type: multipart/alternative; boundary=Apple-Webmail-42--6f4c8431-2b42-4a91-86e4-53b12d616c02
MIME-Version: 1.0
X-CLX-UShades: None
X-CLX-UnSpecialScore: None
X-CLX-Spam: false
X-MANTSH: 1TFkXGxoaHBEKWUQXYllDSxpBbUFzX0QRCllNF2JLRxEKWUkXEQpZXhdjbnkRCkN
 OF11YTlxdQVNiHWMafVBMGkVobktJSRoYRh11HFtkQX9BEQpYXBcZBBoEHxIFGxoaBB0bBBscG
 gQTGxAbHhofGhEKXlkXQhxIGEIRCkNaFxgfHwQYHx8EGB8fBBgfHxEKXk4XGxEKQkUXbFNzeGt
 CfQVTSRgRCkJOF2tFGlJQHkNcWVxoEQpCTBdveX5dHh0TWl4ZQREKQmwXb0FEfVxTBVNdYU4RC
 kJAF2VdZ0ZhAW1SRnAfEQpCWBdveX5dHh0TWl4ZQREKRUMXGxEKcGgXZQUTeBl+f1hGQ3gQGhE
 KcGwXa0xhGnlHT0kfTRoQGhEKbX4XGhEKWE0XSxE=
X-Proofpoint-GUID: wrdvwkyH7I0Wzf0oBDacc02l7_6qNkUk
X-Authority-Info-Out: v=2.4 cv=EY/FgfmC c=1 sm=1 tr=0 ts=6ab41906
 cx=c_apl:c_pps:t_out a=T0mU/zd77MEem+zjWCBf6Q==:117 a=VdqzKS8jKosA:10
 a=x7bEGLp0ZPQA:10 a=CmXsj8IzMh0A:10 a=VkNPw1HP01LnGYTKEx00:22
 a=IvKQ6HjZ87TGgGuGeyAA:9 a=QEXdDO2ut3YA:10 a=_W_S_7VecoQA:10
X-Proofpoint-ORIG-GUID: wrdvwkyH7I0Wzf0oBDacc02l7_6qNkUk
X-Proofpoint-Spam-Details-Enc: AW1haW4tMjYwOTIzMDA3MyBTYWx0ZWRfXwSKfjuTiONaf
 CTZcGjm1WC6NeEOH7+IbWuU9ocehbiBajZ3OYjosvblLr8BNgHK3RWEdiqBLbQSaGcHuv3VaZZT
 S4fNBC4hRlt8RngwEJQkD4EoWflTbeqKvEYLZeagNTzy4a6Z+pbSoXCVKzLZqBFvyH+v9xJAnks
 u7mE0tFZyoLjLErDKI8Rjq9rsrV5fXfzDRAtRopCTZCbBGZDut5hq0c36BRf3hXD+hU/UFPFR8h
 creOQwj6vbpFUG3rt6XPZ/3BxTQN2lKDbrtRTGf0TZUne+PcrDv62aJUOuzgzKEcyOK7OiLWlmI
 d+gNj97FfvpnRWAJnJRQxFswIfkq0eWOEiuUx0RiWuc2hUA1Tdo1rBWX7oALtI=
X-JNJ: AAAAAAABvc24NiGGvQ7ojE4RLEd7kei0KqUaHtHWTCrzQ20ewUPfUS4phFJqm2EvRFZtIbP7pOQ/UL7LGPLlYZPB6A8E+4mnJv2neQ7OG/DmtxatyF0aoOvSXzkbBb0PM/DUev8Zqap7/4CiLYGcFbIi9/d5K279RcJIy2WSk5fJxDJcL9pxIKhGX+5hTJ3nbPPkUIrv92u8mCv6zbosHUw6nTJa5gIhTcbMCdoZwUpXXUeudHVVxK9vKcGJu4UQbYsMX3aPPenCiXv1uOGyWvndz8rFYPqf1RHr3Ml8jF/zug6GvKdWx2Qm8TcnL+LvE+HJfzd0l/aGmKGcPhrmooEBvaY1cM88uynQc1VkK/7Q6G0EWd8tYBlntZa/XzJjgo/7rKYWQKfWFqpP3BdmhxyiHMY7FDW8eBwnOxD3BIa9SEgF4k/IKsPT93g31EhoTD4TLvML6e8n25qfi7/8mGTk+Vos5SAdE2UufLrPKVTHqownP7fdV94vJItvgwODEHbJVx7yi7t/vORzUdnoYoVH1Y5XjCuYWk6BPqbsHsodCOLKhSPwB1s9mNbvcVKW8DNQ8KyqL2wgzn9P05ki/S3F73PqL3IL5uffUzfJZYHRFgld30rKK5chFjzZlDiAY0ytzxwkjUfz5slMBE7x5ttJa3RhuClLT9fMliJGxEFM3PdI3HrgHypLOxWixn9ATRZeZBrwYWm6/pZ5dHpoig5pEXnlgfqozumbjENwZviZifeuJzv/V0+F+Eiy3g8od2f9+TvECy2orQzKqkwYXxm+N4lLms9bha9uNXTX9SCHmCxPPJFDGyvC5K7qdda9VVZklWxDJqec0bnbx+gHFUCJnviG8WnLiT6OVJQgRtPNEtG99BImDA0zA2Y84nnzt/dpgGre1bKjeDwbtTq+/eVd0dQGJjvtvDe4dRcHT30irMrBU5VghDdSzjT6UnyP+LqIdrh4V97DRrBwbZfh4Ua0+CW6aJ7n5WZvigSnC7CZBe5muLuPynPVTOoU7mc3mFIeWlG/NaVHcXmY/RKaq80OHpqC25g30NM7xiMse/tJdAgqHegl9ih2++yNOfcWM6MRCHEnqNbKX1KFhX+KKNVqOx57geBM7Q5jGHN06iMtr8cMckqoKrgLdfSuLSplJ0NV


--Apple-Webmail-42--6f4c8431-2b42-4a91-86e4-53b12d616c02
Content-Transfer-Encoding: quoted-printable
Content-Type: text/plain;
    charset=utf-8;
    format=flowed

Are you reading this in a terminal?
--Apple-Webmail-42--6f4c8431-2b42-4a91-86e4-53b12d616c02
Content-Type: multipart/related;
    type="text/html";
    boundary=Apple-Webmail-86--6f4c8431-2b42-4a91-86e4-53b12d616c02


--Apple-Webmail-86--6f4c8431-2b42-4a91-86e4-53b12d616c02
Content-Transfer-Encoding: quoted-printable
Content-Type: text/html;
    charset=utf-8;

<html><body><div>Are you reading this in a terminal?</div></body></html>
--Apple-Webmail-86--6f4c8431-2b42-4a91-86e4-53b12d616c02--

--Apple-Webmail-42--6f4c8431-2b42-4a91-86e4-53b12d616c02--
```

You can find the parts of the email if you look hard enough, but obviously there is much more work to be done.