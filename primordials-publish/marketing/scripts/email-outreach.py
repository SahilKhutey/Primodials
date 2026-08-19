#!/usr/bin/env python3
"""
Primordials — Press Outreach Emailer
Sends personalized outreach emails using a CSV of contacts and a template.

Usage:
    python email-outreach.py --tier 1 --dry-run
"""

import argparse
import csv
import os
import smtplib
import ssl
import sys
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from pathlib import Path

# Ensure UTF-8 output handling on Windows consoles
if sys.stdout.encoding != 'utf-8':
    try:
        sys.stdout.reconfigure(encoding='utf-8')
    except Exception:
        pass

ROOT = Path(__file__).parent.parent
TEMPLATES = ROOT / 'templates'
CONTACTS = ROOT / 'contacts'

def load_template(name: str):
    template_path = TEMPLATES / f'{name}.md'
    with open(template_path, 'r', encoding='utf-8') as f:
        return f.read()

def load_contacts(tier: int) -> list[dict]:
    contacts_file = CONTACTS / f'tier-{tier}.csv'
    with open(contacts_file, 'r', encoding='utf-8') as f:
        return list(csv.DictReader(f))

def render_email(contact: dict, template_str: str) -> tuple[str, str]:
    body = template_str
    replacements = {
        '{{ name }}': contact.get('name', 'there'),
        '{{ outlet }}': contact.get('outlet', 'your publication'),
        '{{ angle }}': contact.get('angle', 'indie simulations'),
        '{{ article }}': contact.get('article', 'your recent work'),
    }
    for key, val in replacements.items():
        body = body.replace(key, val)
    
    subject = contact.get('subject', '[Press] Primordials — desktop ecosystem wallpaper (review key)')
    return subject, body

def send_email(sender: str, recipient: str, subject: str, body_plain: str,
               smtp_host: str, smtp_port: int, smtp_user: str, smtp_pass: str) -> None:
    msg = MIMEMultipart('alternative')
    msg['Subject'] = subject
    msg['From'] = sender
    msg['To'] = recipient
    msg.attach(MIMEText(body_plain, 'plain', 'utf-8'))

    context = ssl.create_default_context()
    with smtplib.SMTP(smtp_host, smtp_port) as server:
        server.starttls(context=context)
        server.login(smtp_user, smtp_pass)
        server.send_message(msg)

def main() -> int:
    parser = argparse.ArgumentParser(description='Automate press outreach')
    parser.add_argument('--tier', type=int, default=1, help='Outreach tier (1, 2, or 3)')
    parser.add_argument('--dry-run', action='store_true', default=True, help='Print preview without sending')
    parser.add_argument('--limit', type=int, default=0, help='Limit email count')
    args = parser.parse_args()

    template_name = {1: 'tier-1-personal', 2: 'tier-2-semi', 3: 'tier-3-mass'}.get(args.tier, 'tier-1-personal')
    template_str = load_template(template_name)
    contacts = load_contacts(args.tier)

    if args.limit:
        contacts = contacts[:args.limit]

    sender = os.environ.get('OUTREACH_EMAIL', 'hello@primordials.dev')
    print(f'[+] Tier {args.tier} Outreach: {len(contacts)} contacts loaded')
    print(f'[+] Sender: {sender}')
    print(f'[+] Mode: {"DRY RUN (Preview Only)" if args.dry_run else "LIVE SEND"}')
    print('-' * 60)

    for contact in contacts:
        subject, body = render_email(contact, template_str)
        recipient = contact.get('email', '')
        if not recipient:
            continue

        if args.dry_run:
            print(f'\n-- To: {recipient} ({contact.get("outlet")}) --')
            print(f'Subject: {subject}')
            print(f'\n{body[:250]}...\n')
        else:
            smtp_host = os.environ.get('SMTP_HOST', 'smtp.gmail.com')
            smtp_port = int(os.environ.get('SMTP_PORT', '587'))
            smtp_user = os.environ.get('SMTP_USER', sender)
            smtp_pass = os.environ.get('SMTP_PASS', '')
            try:
                send_email(sender, recipient, subject, body, smtp_host, smtp_port, smtp_user, smtp_pass)
                print(f'[OK] Sent to {recipient}')
            except Exception as e:
                print(f'[FAIL] Failed {recipient}: {e}')

    print('\n[+] Outreach complete.')
    return 0

if __name__ == '__main__':
    sys.exit(main())
