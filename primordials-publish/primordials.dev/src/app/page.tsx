import { Hero } from '@/components/sections/Hero';
import { Features } from '@/components/sections/Features';
import { Biomes } from '@/components/sections/Biomes';
import { Performance } from '@/components/sections/Performance';
import { CTA } from '@/components/sections/CTA';

export default function HomePage() {
  return (
    <>
      <Hero />
      <Features />
      <Biomes />
      <Performance />
      <CTA />
    </>
  );
}
