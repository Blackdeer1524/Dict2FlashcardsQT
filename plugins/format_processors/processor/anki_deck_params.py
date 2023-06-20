import genanki

MODEL_FIELDS = [
    {"name": "Sentence"},
    {"name": "Word"},
    {"name": "Definition"},
    {"name": "Image"},
    {"name": "Word Audio"},
]


MODEL_CSS = """\
.card { 
    font-size: 23px; 
    text-align: left; 
    color: black; 
    background-color: #FFFAF0; 
    margin: 20px auto 20px auto; 
    padding: 0 20px 0 20px; 
    max-width: 600px; 
}

.accent {
    font-size: 40px;
}
"""


# ONE_SENTENCE_RESULTING_MODEL = genanki.Model(
#     1869993568,  # just a random number
#     "Mined Sentence Vocab",
#     fields=MODEL_FIELDS,
#     templates=[
#         {
#             "name": "Recognition",
#             "qfmt": "{{Sentence}}",
#             "afmt": """\
# {{FrontSide}}
# <hr id="answer">
# <div class="accent">
#     {{Word}}
# </div>
# {{Definition}}<br>
# {{Image}}<br>
# {{Word Audio}}<br>
#
# Tags{{#Tags}}|{{/Tags}}{{Tags}}
# """,
#         },
#     ],
#     css=MODEL_CSS,
# )


# https://ankiweb.net/shared/info/1639213385
MERGING_RESULTING_MODEL = genanki.Model(
    1607392319,  # just a random number
    "[Random] Mined Sentence Vocab",
    fields=MODEL_FIELDS,
    templates=[
        {
            "name": "Recognition",
            "qfmt": "{{rand-alg:Sentence}}",
            "afmt": """\
{{Sentence}}
<hr id="answer">
<div class="accent">
    {{Word}}
</div>
{{Definition}}<br>
{{Image}}<br>
{{Word Audio}}<br>

Tags{{#Tags}}|{{/Tags}}{{Tags}}
""",
        },
    ],
    css=MODEL_CSS,
)
