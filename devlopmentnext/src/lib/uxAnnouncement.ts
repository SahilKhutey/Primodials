export type Announcement = {
  id: string;
  title: string;
  body: string;
  tone: 'info' | 'success' | 'warning';
};

const STORAGE_KEY = 'polygonal-primordials.dismissed-announcements.v1';

function readDismissed(): string[] {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (!raw) return [];
    const parsed = JSON.parse(raw);
    return Array.isArray(parsed) ? parsed.filter((v) => typeof v === 'string') : [];
  } catch {
    return [];
  }
}

export function isAnnouncementDismissed(id: string): boolean {
  return readDismissed().includes(id);
}

export function dismissAnnouncement(id: string): void {
  const current = readDismissed();
  if (current.includes(id)) return;

  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify([...current, id]));
  } catch {
    // Non-critical.
  }
}
