
import os

for root, _, files in os.walk('Games/PolygonalPrimordials/src'):
    for f in files:
        if not f.endswith('.h') and not f.endswith('.cpp'): continue
        path = os.path.join(root, f)
        with open(path, 'r', encoding='utf-8') as file:
            content = file.read()
        
        if '#include \"Shape/' in content:
            content = content.replace('#include \"Shape/', '#include \"')
            content = content.replace('.h\"', '.hpp\"')
            
            with open(path, 'w', encoding='utf-8') as file:
                file.write(content)

