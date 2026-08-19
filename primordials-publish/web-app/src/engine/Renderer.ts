/**
 * Canvas 2D renderer for the simulation
 * Optimized for 60 FPS with thousands of entities
 */
import type { Entity } from '@/types/entity';

export class Renderer {
  private ctx: CanvasRenderingContext2D;
  private width: number = 0;
  private height: number = 0;
  private dpr: number;

  constructor(canvas: HTMLCanvasElement) {
    const ctx = canvas.getContext('2d', { alpha: false });
    if (!ctx) throw new Error('Failed to get 2D context');
    this.ctx = ctx;
    this.dpr = typeof window !== 'undefined' ? window.devicePixelRatio || 1 : 1;
    this.resize(canvas);
  }

  resize(canvas: HTMLCanvasElement): void {
    const rect = canvas.getBoundingClientRect();
    this.width = rect.width || 1920;
    this.height = rect.height || 1080;
    canvas.width = this.width * this.dpr;
    canvas.height = this.height * this.dpr;
    this.ctx.setTransform(this.dpr, 0, 0, this.dpr, 0, 0);
  }

  clear(background: string, gradient?: string[]): void {
    if (gradient && gradient.length > 0) {
      const grad = this.ctx.createLinearGradient(0, 0, 0, this.height);
      gradient.forEach((color, i) => {
        grad.addColorStop(i / (gradient.length - 1), color);
      });
      this.ctx.fillStyle = grad;
    } else {
      this.ctx.fillStyle = background;
    }
    this.ctx.fillRect(0, 0, this.width, this.height);
  }

  drawEntities(entities: Entity[]): void {
    for (const entity of entities) {
      this.drawEntity(entity);
    }
  }

  private drawEntity(entity: Entity): void {
    const { x, y } = entity.position;
    const size = entity.size;
    const color = entity.color;

    this.ctx.save();
    this.ctx.translate(x, y);
    this.ctx.rotate(entity.heading);

    switch (entity.shape) {
      case 'circle':
        this.drawCircle(size, color);
        break;
      case 'triangle':
        this.drawTriangle(size, color);
        break;
      case 'hexagon':
        this.drawHexagon(size, color);
        break;
      case 'square':
        this.drawSquare(size, color);
        break;
      case 'star':
        this.drawStar(size, color);
        break;
      default:
        this.drawCircle(size, color);
    }
    this.ctx.restore();
  }

  private drawCircle(size: number, color: string): void {
    this.ctx.fillStyle = color;
    this.ctx.beginPath();
    this.ctx.arc(0, 0, size, 0, Math.PI * 2);
    this.ctx.fill();
  }

  private drawTriangle(size: number, color: string): void {
    this.ctx.fillStyle = color;
    this.ctx.beginPath();
    this.ctx.moveTo(size, 0);
    this.ctx.lineTo(-size * 0.7, -size * 0.7);
    this.ctx.lineTo(-size * 0.7, size * 0.7);
    this.ctx.closePath();
    this.ctx.fill();
  }

  private drawHexagon(size: number, color: string): void {
    this.ctx.fillStyle = color;
    this.ctx.beginPath();
    for (let i = 0; i < 6; i++) {
      const angle = (i * Math.PI) / 3;
      const x = Math.cos(angle) * size;
      const y = Math.sin(angle) * size;
      if (i === 0) this.ctx.moveTo(x, y);
      else this.ctx.lineTo(x, y);
    }
    this.ctx.closePath();
    this.ctx.fill();
  }

  private drawSquare(size: number, color: string): void {
    this.ctx.fillStyle = color;
    this.ctx.fillRect(-size * 0.7, -size * 0.7, size * 1.4, size * 1.4);
  }

  private drawStar(size: number, color: string): void {
    this.ctx.fillStyle = color;
    this.ctx.beginPath();
    const spikes = 5;
    const outer = size;
    const inner = size * 0.5;
    for (let i = 0; i < spikes * 2; i++) {
      const r = i % 2 === 0 ? outer : inner;
      const angle = (i * Math.PI) / spikes - Math.PI / 2;
      const x = Math.cos(angle) * r;
      const y = Math.sin(angle) * r;
      if (i === 0) this.ctx.moveTo(x, y);
      else this.ctx.lineTo(x, y);
    }
    this.ctx.closePath();
    this.ctx.fill();
  }

  getDimensions(): { width: number; height: number } {
    return { width: this.width, height: this.height };
  }
}
