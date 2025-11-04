package main

import (
	"fmt"
	"image"
	"image/draw"
	"image/png"
	"io/ioutil"
	"log"
	"math"
	"os"
    "strings"
)

func main() {
	// Open the current directory
	files, err := ioutil.ReadDir(".")
	if err != nil {
		log.Fatal(err)
	}

	var images []image.Image
    var imageNames []string
	// Loop through the files, looking for png files
	for _, file := range files {
		if !file.IsDir() && len(file.Name()) > 4 && file.Name()[len(file.Name())-4:] == ".png" {
            if file.Name() == "tilemap.png" {
                continue
            }
			// Open the file
			f, err := os.Open(file.Name())
			if err != nil {
				log.Printf("could not open file %s: %v\n", file.Name(), err)
				continue
			}

			// Decode the image
			img, err := png.Decode(f)
			if err != nil {
				log.Printf("could not decode image %s: %v\n", file.Name(), err)
                f.Close()
				continue
			}
			images = append(images, img)
            imageNames = append(imageNames, file.Name())
            f.Close()
		}
	}

	if len(images) == 0 {
		log.Fatal("No PNG images found in the current directory.")
	}

	// Calculate the size of the tilemap
	numImages := len(images)
	gridSize := int(math.Ceil(math.Sqrt(float64(numImages))))
	tilemapWidth := gridSize * 64
	tilemapHeight := gridSize * 64

	// Create a new RGBA image for the tilemap
	tilemap := image.NewRGBA(image.Rect(0, 0, tilemapWidth, tilemapHeight))

    // Create the output file for the positions
    posFile, err := os.Create("tilemap.txt")
    if err != nil {
        log.Fatal(err)
    }
    defer posFile.Close()

	// Draw each image onto the tilemap
	for i, img := range images {
		x := (i % gridSize) * 64
		y := (i / gridSize) * 64
		rect := image.Rect(x, y, x+64, y+64)
		draw.Draw(tilemap, rect, img, image.Point{0, 0}, draw.Src)

        // Write the position and name to the text file
        name := strings.TrimSuffix(imageNames[i], ".png")
        posString := fmt.Sprintf("%s %d %d\n", name, x, y)
        _, err := posFile.WriteString(posString)
        if err != nil {
            log.Printf("could not write to position file: %v\n", err)
        }
	}

	// Create the output file
	outFile, err := os.Create("tilemap.png")
	if err != nil {
		log.Fatal(err)
	}
	defer outFile.Close()

	// Encode the tilemap to the output file
	err = png.Encode(outFile, tilemap)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("Tilemap and position file created successfully: tilemap.png, tilemap.txt")
}