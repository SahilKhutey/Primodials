import { ChemicalField } from '../src/sim/chemicalField';

function testTC_D2() {
  const field = new ChemicalField(400, 300, 20);
  field.depositAttractant(100, 100, 5.5);
  field.depositRepellent(100, 100, 2.3);

  const beforeAtt = field.sampleAttractant(100, 100).value;
  const beforeRep = field.sampleRepellent(100, 100).value;

  console.log(`Before resize: attractant=${beforeAtt}, repellent=${beforeRep}`);

  // Expand world
  field.resize(600, 450);

  const afterAtt = field.sampleAttractant(100, 100).value;
  const afterRep = field.sampleRepellent(100, 100).value;

  console.log(`After resize: attractant=${afterAtt}, repellent=${afterRep}`);

  if (Math.abs(beforeAtt - afterAtt) < 0.0001 && Math.abs(beforeRep - afterRep) < 0.0001 && afterAtt > 0) {
    console.log('TC-D2 VERIFIED PASS: Chemical field values successfully preserved across world expansion!');
  } else {
    console.error('TC-D2 FAIL: Chemical field data was lost or corrupted during resize');
    process.exit(1);
  }
}

testTC_D2();
