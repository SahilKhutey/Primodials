import { PRODUCT_NAME, PRODUCT_VERSION } from "./version";

export type PreflightCheck = {
  id: string;
  label: string;
  ok: boolean;
  detail: string;
};

function checkStorage(): boolean {
  try {
    const key = "__pp_preflight__";
    window.localStorage.setItem(key, "1");
    window.localStorage.removeItem(key);
    return true;
  } catch {
    return false;
  }
}

export function runReleasePreflight(): PreflightCheck[] {
  const canvas = document.createElement("canvas");
  return [
    { id: "product", label: "Product identity", ok: PRODUCT_NAME === "Polygonal Primordials", detail: PRODUCT_NAME },
    { id: "version", label: "Product version", ok: PRODUCT_VERSION.length > 0, detail: PRODUCT_VERSION },
    { id: "canvas", label: "2D canvas", ok: !!canvas.getContext("2d"), detail: "Canvas 2D available" },
    { id: "storage", label: "Local persistence", ok: checkStorage(), detail: "localStorage read/write" },
    { id: "viewport", label: "Viewport", ok: window.innerWidth >= 640 && window.innerHeight >= 360, detail: `${window.innerWidth}×${window.innerHeight}` },
  ];
}
