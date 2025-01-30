# Fight Night Finale Engine
GAM150 project engine

## Coding conventions
### Naming conventions
- Namespaces use `snake_case`
- Classes and Functions use `PascalCase`
- Variables use `camelCase`
    - Private and Protected variables must be prefixed with `m_variableName`

### GameObjects conventions
- Init() -> Used for initializing pointers of the object
- Start() -> Initialize variables (transform, texture...)
- Update() -> Update Logic
- Draw() -> Custom draw Logic
- Destroy() -> Uninitialize logic (Only uninitialize textures if the texture is not going to be used again)

### Code style
- NO TABS, 2 spaces
- Soft line limit of 120 characters
- Hard line limit of 150 characters
- Use `#pragma region` to group functions inside source files if there are a lot of functions
- Favour longer classes

### Documentation
EVERY public namespace, class, function and variable should be documented. Private members aren't necessary but highly recomended for complex functions. Doxygen prefix `@` is prefered over `\`. A good documentation must have a `@brief` explanation and a more detailed one. Comments explaining code should end with a sufix indicating who wrote the comment.
```
-x -> Xein
-m -> Dario
-d -> Dante
-a -> Alexey
```
For code that needs to be reviewed or for notes that are EXTREMELLY important use TODO for easy tracking. Completed tasks should be deleted after.
```
// TODO: your note here
```
Header files must have a header comment that looks like this:
```cpp
/**
 * @file ${FILE_NAME}
 * @author ${USER}
 * @date ${DATE}
 *
 * @brief [Brief description of the file's purpose]
 */
``` 
