import { useEffect, useState } from "react";
import {
  installDisplayMetricsListener,
  readDisplayMetrics,
  type DisplayMetrics,
} from "@/lib/displayMetrics";

export function useDisplayMetrics(): DisplayMetrics {
  const [metrics, setMetrics] = useState<DisplayMetrics>(() => readDisplayMetrics());

  useEffect(() => installDisplayMetricsListener(setMetrics), []);

  return metrics;
}
