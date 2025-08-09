// // package declaration
// package main

// // imports
// import (
// 	"encoding/json"
// 	"fmt"
// 	"os"
// 	"sort"
// )

// // custom type to handle flexible icon field
// type IconField struct {
// 	Single string
// 	Dark   string
// 	Light  string
// }

// // custom unmarshal logic for icon field
// func (i *IconField) UnmarshalJSON(data []byte) error {
// 	// Try unmarshaling as a single string
// 	var single string
// 	if err := json.Unmarshal(data, &single); err == nil {
// 		i.Single = single
// 		return nil
// 	}

// 	// Try unmarshaling as an object with dark/light keys
// 	var dual struct {
// 		Dark  string `json:"dark"`
// 		Light string `json:"light"`
// 	}
// 	if err := json.Unmarshal(data, &dual); err == nil {
// 		i.Dark = dual.Dark
// 		i.Light = dual.Light
// 		return nil
// 	}

// 	return fmt.Errorf("invalid icon format: %s", string(data))
// }

// // type definitions
// type Module struct {
// 	Name        string    `json:"name"`
// 	Description string    `json:"description"`
// 	Path        string    `json:"path"`
// 	Markdown    string    `json:"markdown"`
// 	Tester      string    `json:"tester"`
// 	Example     string    `json:"example"`
// 	Icon        IconField `json:"icon"`
// }
// type Package map[string]Module
// type Library map[string]Package

// // data
// var library Library
// var verbose int

// // indent function for pretty printing
// func indent(depth int, isLast []bool) {
// 	for i := 0; i < depth; i++ {
// 		if i < len(isLast) && isLast[i] {
// 			fmt.Print("    ")
// 		} else {
// 			fmt.Print("│   ")
// 		}
// 	}
// }

// // helper to print leaf fields
// func printField(depth int, isLast []bool, label, value string, isFinal bool) {
// 	fieldIsLast := make([]bool, len(isLast))
// 	copy(fieldIsLast, isLast)
// 	if len(fieldIsLast) <= depth {
// 		for len(fieldIsLast) <= depth {
// 			fieldIsLast = append(fieldIsLast, false)
// 		}
// 	}
// 	fieldIsLast[depth] = isFinal
// 	indent(depth+1, fieldIsLast)
// 	prefix := "├──"
// 	if isFinal {
// 		prefix = "└──"
// 	}
// 	fmt.Printf("%s %s → %s\n", prefix, label, value)
// }

// // method definitions
// func (m Module) ModulePrint(depth int, isLast []bool, isLastModule bool) {
// 	currentIsLast := make([]bool, len(isLast))
// 	copy(currentIsLast, isLast)
// 	if depth > 0 {
// 		for len(currentIsLast) <= depth-1 {
// 			currentIsLast = append(currentIsLast, false)
// 		}
// 		currentIsLast[depth-1] = isLastModule
// 	}

// 	indent(depth, currentIsLast)
// 	if isLastModule {
// 		fmt.Printf("└── 📦 %s\n", m.Name)
// 	} else {
// 		fmt.Printf("├── 📦 %s\n", m.Name)
// 	}

// 	if verbose > depth+1 {
// 		detailIsLast := make([]bool, len(currentIsLast))
// 		copy(detailIsLast, currentIsLast)
// 		for len(detailIsLast) <= depth {
// 			detailIsLast = append(detailIsLast, false)
// 		}
// 		detailIsLast[depth] = isLastModule

// 		printField(depth, detailIsLast, "💬 description", m.Description, false)
// 		printField(depth, detailIsLast, "📄 markdown   ", ifEmpty(m.Markdown), false)
// 		printField(depth, detailIsLast, "🚀 example    ", ifEmpty(m.Example), false)
// 		printField(depth, detailIsLast, "🧪 tester     ", ifEmpty(m.Tester), false)

// 		switch {
// 		case m.Icon.Single != "":
// 			printField(depth, detailIsLast, "🖼️  icon       ", m.Icon.Single, false)
// 		case m.Icon.Dark != "" || m.Icon.Light != "":
// 			indent(depth+1, detailIsLast)
// 			fmt.Printf("├── 🖼️  icon\n")

// 			subDetail := make([]bool, len(detailIsLast))
// 			copy(subDetail, detailIsLast)
// 			for len(subDetail) <= depth+1 {
// 				subDetail = append(subDetail, false)
// 			}
// 			subDetail[depth+1] = false
// 			printField(depth+1, subDetail, "⚪ light    ", m.Icon.Light, false)
// 			subDetail[depth+1] = true
// 			printField(depth+1, subDetail, "⚫ dark     ", m.Icon.Dark, true)
// 		default:
// 			printField(depth, detailIsLast, "🖼️  icon       ", "❌", false)
// 		}

// 		printField(depth, detailIsLast, "🗂️  path       ", m.Path, true)
// 	}
// }

// func (p Package) PackagePrint(name string, depth int, isLast []bool, isLastPackage bool) {
// 	currentIsLast := make([]bool, len(isLast))
// 	copy(currentIsLast, isLast)
// 	if depth > 0 {
// 		for len(currentIsLast) <= depth-1 {
// 			currentIsLast = append(currentIsLast, false)
// 		}
// 		currentIsLast[depth-1] = isLastPackage
// 	}

// 	indent(depth, currentIsLast)
// 	if isLastPackage {
// 		fmt.Printf("└── 🎁 %s\n", name)
// 	} else {
// 		fmt.Printf("├── 🎁 %s\n", name)
// 	}

// 	if verbose > depth {
// 		pkgSlice := make([]Module, 0, len(p))
// 		for _, mod := range p {
// 			pkgSlice = append(pkgSlice, mod)
// 		}

// 		sort.Slice(pkgSlice, func(i, j int) bool {
// 			return pkgSlice[i].Name < pkgSlice[j].Name
// 		})

// 		moduleIsLast := make([]bool, len(currentIsLast))
// 		copy(moduleIsLast, currentIsLast)
// 		for len(moduleIsLast) <= depth {
// 			moduleIsLast = append(moduleIsLast, false)
// 		}
// 		moduleIsLast[depth] = isLastPackage

// 		for i := 0; i < len(pkgSlice)-1; i++ {
// 			pkgSlice[i].ModulePrint(depth+1, moduleIsLast, false)
// 		}
// 		if len(pkgSlice) > 0 {
// 			pkgSlice[len(pkgSlice)-1].ModulePrint(depth+1, moduleIsLast, true)
// 		}
// 	}
// }

// func (l Library) LibraryPrint(name string, depth int) {
// 	fmt.Printf("📚 %s\n", name)

// 	if verbose > depth {
// 		libSlice := make([]string, 0, len(l))
// 		for packageName := range l {
// 			libSlice = append(libSlice, packageName)
// 		}

// 		sort.Strings(libSlice)

// 		var isLast []bool
// 		for i := 0; i < len(libSlice)-1; i++ {
// 			l[libSlice[i]].PackagePrint(libSlice[i], depth, isLast, false)
// 		}
// 		if len(libSlice) > 0 {
// 			l[libSlice[len(libSlice)-1]].PackagePrint(libSlice[len(libSlice)-1], depth, isLast, true)
// 		}
// 	}
// }

// func ifEmpty(s string) string {
// 	if s == "" {
// 		return "❌"
// 	}
// 	return s
// }

// func ok(e error) {
// 	if e != nil {
// 		panic(e)
// 	}
// }

// func init() {
// 	var json_text []byte
// 	var err error

// 	json_text, err = os.ReadFile("g.json")
// 	ok(err)

// 	err = json.Unmarshal(json_text, &library)
// 	ok(err)

// 	if len(os.Args) > 1 {
// 		fmt.Sscanf(os.Args[1], "%d", &verbose)
// 	} else {
// 		verbose = 0
// 	}
// }

// func main() {
// 	library.LibraryPrint("gsdk", 0)
// }
